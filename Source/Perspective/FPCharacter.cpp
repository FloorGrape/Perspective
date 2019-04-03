// Fill out your copyright notice in the Description page of Project Settings.

#include "FPCharacter.h"
#include "Perspective.h"
#include "Components/InputComponent.h"
#include <string>
#include "Kismet/KismetMathLibrary.h"

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
	forwardVec = cameraComponent->GetForwardVector();
	end = start + forwardVec * maxInteractionDist;
	
	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.1f, 0, 1);

	if (!bHoldingItem)
	{
		if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, defaultComponentQueryParams, defaultResponseParams))
		{
			currentItem = Cast<AInteractableObject>(hit.GetActor());

			if (currentItem) 
			{
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
			FVector origin;
			FVector boxExtent;
			currentItem->GetActorBounds(false, origin, boxExtent);
			FVector st = currentItem->GetActorLocation() + (forwardVec * boxExtent.X);
			FVector ed = st + forwardVec * 50000.0f;
			FHitResult ht;

			if (GetWorld()->LineTraceSingleByChannel(ht, st, ed, ECC_Visibility, defaultComponentQueryParams, defaultResponseParams))
			{
				
				DrawDebugLine(GetWorld(), st, ed, FColor::Green, false, 0.1f, 0, 1);
				
				distance = hit.Distance + ht.Distance;


				OnForcePerspective(static_cast<AInteractableObject*>(hit.GetActor()));
			}
		}
	}

	if (bInspecting)
	{
		if (bHoldingItem)
		{
			cameraComponent->SetFieldOfView(FMath::Lerp(cameraComponent->FieldOfView, 90.0f, 0.1f));

			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.90000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.90000002f;
			currentItem->RotateActor();
		}
	}
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
	cameraComponent->bUsePawnControlRotation = !cameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void AFPCharacter::ToggleItemPickup()
{
	if(currentItem)
	{
		bHoldingItem = !bHoldingItem;
		currentItem->Pickup();

		if(!bHoldingItem)
		{
			currentItem = nullptr;
		}
	}
}

void AFPCharacter::OnForcePerspective(AInteractableObject* object_)
{
	float angularSize;
	float dist = distance;
	FVector scale = object_->GetOriginScale();
	float size = scale.Y;
	float newSize;
	float scaleFactor;

	angularSize = 2 * UKismetMathLibrary::DegAtan(scale.Y/(2 * dist));

	newSize = 2 * UKismetMathLibrary::DegTan(angularSize / 2) * dist;

	scaleFactor = newSize / size;


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), scale));
	}

	object_->UpdateScale(scaleFactor);
}
