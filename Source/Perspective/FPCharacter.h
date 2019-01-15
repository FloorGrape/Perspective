// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"


UCLASS()
class PERSPECTIVE_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()
		
public:
	// Sets default values for this character's properties
	AFPCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float deltaTime_) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent_) override;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* visibleComponent;*/

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* cameraComponent;*/

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
	
private:
	float maxWalkSpeed = 1.0f;
	float sprintModifier = 5.0f;
};
