// Fill out your copyright notice in the Description page of Project Settings.

#include "FPCharacter.h"
#include "Perspective.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <string>
#include <OpenGL/GLES2/gl2ext.h>

// Sets default values
AFPCharacter::AFPCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//RootComponent = visibleComponent;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	cameraComponent->SetupAttachment(GetCapsuleComponent());
	cameraComponent->SetRelativeLocation(FVector(10.0f, 0.0f, 30.0f + BaseEyeHeight));
	cameraComponent->bUsePawnControlRotation = true;

	holdingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	holdingComponent->SetupAttachment(cameraComponent);
	holdingComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	currentItem = nullptr;
	bCanMove = true;
	bInspecting = false;
	distance = 0.0f;
}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
	}

	pitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	pitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;
}

// Called every frame
void AFPCharacter::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);
	start = cameraComponent->GetComponentLocation(); 
	end = start + forwardVec * maxInteractionDist;
	forwardVec = cameraComponent->GetForwardVector();


	if (!bHoldingItem)
	{
		if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, defaultCollisionQueryParams, defaultResponseParams))
		{
			currentItem = Cast<AInteractableObject>(hit.GetActor());

			if (currentItem) 
			{
				end = start + forwardVec * FVector(currentItem->GetActorLocation() - GetTransform().GetLocation()).Size();
				DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.1f, 0, 1);

				FVector localCoord = GetTransform().InverseTransformPosition(hit.GetActor()->GetActorLocation());
				holdingComponent->SetRelativeLocation(FVector(localCoord.X, 0.0f, 0.0f));
			}
		}
		else
		{
			currentItem = nullptr;
		}
	}
	else if (bHoldingItem)
	{
		if (currentItem)
		{
			holdingCollisionQueryParams.AddIgnoredActor(currentItem);
			end = start + forwardVec * maxInteractionDist;

			if (GetWorld()->LineTraceSingleByChannel(hitBlue, start, end, ECC_Visibility, holdingCollisionQueryParams, defaultResponseParams))
			{
				DrawDebugLine(GetWorld(), start, static_cast<FVector>(hitBlue.ImpactPoint), FColor::Blue, false, 0.1f, 0, 1);
				
				angularSize = AngularSize();
				OnForcePerspective(currentItem, angularSize);
			}
		}
	}

	if (bInspecting)
	{
		if (bHoldingItem)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.90000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.90000002f;
			currentItem->RotateActor();
		}
	}
}

void AFPCharacter::OnForcePerspective(AInteractableObject* object_, float& angSize_)
{
	FVector endLoc;

	FHitResult downHit;
	FVector st = object_->GetRootComponent()->GetComponentLocation();
	FVector ed = st + FVector(0, 0, -1.001f) * object_->GetRootComponent()->Bounds.SphereRadius;

	if (GetWorld()->LineTraceSingleByChannel(downHit, st, ed, ECC_Visibility, holdingCollisionQueryParams, defaultResponseParams))
	{
		endLoc = GroundCheck(downHit);
	}
	else
	{
		endLoc = static_cast<FVector>(hitBlue.ImpactPoint + (hitBlue.ImpactNormal * currentItem->GetRootComponent()->Bounds.SphereRadius));
	}

	endLoc = static_cast<FVector>(hitBlue.ImpactPoint + (hitBlue.ImpactNormal * object_->GetRootComponent()->Bounds.SphereRadius));
	
	float dist = FMath::Abs((endLoc - cameraComponent->GetComponentLocation()).Size());
	float newBoundRad;
	float boundDiam = 2 * object_->GetRootComponent()->Bounds.SphereRadius;

	newBoundRad = 2 * (UKismetMathLibrary::DegTan(angSize_/2) * dist);

	itemScaleFactor = newBoundRad / (boundDiam * 2);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("scaleFactor: %f"), itemScaleFactor));
	}

	object_->SetScaleFactor(itemScaleFactor);
	object_->SetActorLocation(endLoc);
}

FVector AFPCharacter::GroundCheck(FHitResult ht_)
{
	float hyp = ht_.Distance;
	float adj = cameraComponent->GetComponentTransform().GetLocation().Z;
	float uAng = UKismetMathLibrary::DegAcos(adj/hyp);
	float oAng = 180 - 90 - uAng;

	float height = currentItem->GetRootComponent()->Bounds.SphereRadius;

	float offset = height/UKismetMathLibrary::DegTan(oAng) + height;
	FVector dir = hitBlue.TraceStart - hitBlue.ImpactPoint;
	dir.Normalize();

	return static_cast<FVector>(ht_.ImpactPoint + (dir * offset));
}

float AFPCharacter::AngularSize()
{
	FVector displacement = currentItem->GetActorLocation() - cameraComponent->GetComponentLocation();
	float initDist = FMath::Abs(displacement.Size());
	float boundDiam = 2 * currentItem->GetRootComponent()->Bounds.SphereRadius;

	 return (2 * UKismetMathLibrary::DegAtan2(boundDiam, (2 * initDist)));
}


// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent_)
{
	Super::SetupPlayerInputComponent(playerInputComponent_);

	//Players movement bindings.
	playerInputComponent_->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);
	playerInputComponent_->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);

	//Players look bindings.
	playerInputComponent_->BindAxis("Turn", this, &AFPCharacter::AddControllerYawInput);
	playerInputComponent_->BindAxis("LookUp", this, &AFPCharacter::AddControllerPitchInput);

	//Players jump bindings.
	playerInputComponent_->BindAction("Jump", IE_Pressed, this, &AFPCharacter::StartJump);
	playerInputComponent_->BindAction("Jump", IE_Released, this, &AFPCharacter::StopJump);

	playerInputComponent_->BindAction("Run", IE_Pressed, this, &AFPCharacter::StartSprint);
	playerInputComponent_->BindAction("Run", IE_Released, this, &AFPCharacter::StopSprint);

	playerInputComponent_->BindAction("Interact", IE_Pressed, this, &AFPCharacter::OnInteract);
	playerInputComponent_->BindAction("Inspect", IE_Pressed, this, &AFPCharacter::OnInspect);
	playerInputComponent_->BindAction("Inspect", IE_Released, this, &AFPCharacter::OnInspectReleased);
}

void AFPCharacter::MoveForward(float value_)
{
	if(value_ != 0.0f && bCanMove)
	{
		// Find out which way is "forward" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, value_);
	}
}

void AFPCharacter::MoveRight(float value_)
{
	if(value_ != 0.0f && bCanMove)
	{
		// Find out which way is "right" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, value_);
	}
}

void AFPCharacter::StartJump() 
{
	bPressedJump = true;
}

void AFPCharacter::StopJump() 
{
	bPressedJump = false;
}

void AFPCharacter::StartSprint() 
{
	GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed * sprintModifier;
}

void AFPCharacter::StopSprint() 
{
	GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed;
}

void AFPCharacter::OnInteract()
{
	if(currentItem && !bInspecting)
	{
		ToggleItemPickup();
	}
}

void AFPCharacter::OnInspect()
{
	if(bHoldingItem)
	{
		lastRotation = GetControlRotation();
		ToggleMovement();
	}
	else
	{
		bInspecting = true;
	}
}

void AFPCharacter::OnInspectReleased()
{
	if(bInspecting && bHoldingItem)
	{
		GetController()->SetControlRotation(lastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = pitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = pitchMin;

		ToggleMovement();
	}
	else
	{
		bInspecting = false;
	}
}

void AFPCharacter::ToggleMovement()
{
	bCanMove = !bCanMove;
	bInspecting = !bInspecting;
	currentItem->SetInspecting(bInspecting);
	cameraComponent->bUsePawnControlRotation = !cameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void AFPCharacter::ToggleItemPickup()
{
	if(currentItem)
	{
		bHoldingItem = !bHoldingItem;

		if(bHoldingItem)
		{
		}

		currentItem->Pickup();

		if(!bHoldingItem)
		{
			currentItem = nullptr;
		}
	}
}
