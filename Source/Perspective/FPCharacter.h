// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Switch.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

#define MAX_WALK_SPD 1.0f
#define SPRINT_MOD 5.0f


UCLASS()
class PERSPECTIVE_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()
		
public:
	// Sets default values for this character's properties
	AFPCharacter();

protected:

	UPROPERTY()
		float maxWalkSpeed = MAX_WALK_SPD;

	UPROPERTY()
		float sprintModifier = SPRINT_MOD;

	UPROPERTY(EditAnywhere)
		UCameraComponent* cameraComponent;

	UPROPERTY(EditAnywhere)
		ASwitch* currentSwitch;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float deltaTime_) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent_) override;

	//Handles forward and backward movement
	UFUNCTION()
		void MoveForward(float value_);

	// Handles right and left movement.
	UFUNCTION()
		void MoveRight(float value_);

	//Handles jumping
	//Sets jump flag when key is pressed.
	UFUNCTION()
		void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
		void StopJump();

	//Handles player running
	UFUNCTION()
		void StartSprint();

	UFUNCTION()
		void StopSprint();

	UFUNCTION()
		void CastRay();
};
