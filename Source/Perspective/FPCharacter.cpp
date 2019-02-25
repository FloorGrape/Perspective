// Fill out your copyright notice in the Description page of Project Settings.

#include "FPCharacter.h"
#include "Perspective.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
	}

	maxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called every frame
void AFPCharacter::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);

	CastRay();
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

	playerInputComponent_->BindAction("Interact", IE_Pressed, this, &AFPCharacter::CastRay);
}

void AFPCharacter::MoveForward(float value_)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value_);
}

void AFPCharacter::MoveRight(float value_)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value_);
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

void AFPCharacter::CastRay()
{
	FVector startLocation_ = cameraComponent->GetComponentLocation();
	FVector endLocation_ = startLocation_ + (cameraComponent->GetForwardVector() * maxInteractionDist);

	UKismetSystemLibrary::DrawDebugLine(GetWorld(), startLocation_, endLocation_, FColor::Red, 1.0f, 3.0f);

	FHitResult hit_;
	FCollisionQueryParams collisionParams_;
	collisionParams_.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(hit_, startLocation_, endLocation_, ECC_Visibility, collisionParams_))
	{
		if (hit_.GetActor()->GetClass()->IsChildOf(ASwitch::StaticClass()))
		{

		}
	}
}
