// Fill out your copyright notice in the Description page of Project Settings.

#include "FPCharacter.h"
#include "Perspective.h"
#include "Components/InputComponent.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Players movement bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);

	//Players look bindings.
	PlayerInputComponent->BindAxis("Turn", this, &AFPCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPCharacter::AddControllerPitchInput);

	//Players jump bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPCharacter::StopJump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFPCharacter::StartSprint);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AFPCharacter::StopSprint);

}

void AFPCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFPCharacter::StartJump() {
	bPressedJump = true;
}

void AFPCharacter::StopJump() {
	bPressedJump = false;
}

void AFPCharacter::StartSprint() {
	
	GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed * sprintModifier;
}

void AFPCharacter::StopSprint() {

	GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed;

}