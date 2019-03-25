// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Switch.h"
#include "InteractableObject.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

UCLASS()
class PERSPECTIVE_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY()
		float maxWalkSpeed = 1.0f;

	UPROPERTY()
		float sprintModifier = 5.0f;

	UPROPERTY()
		float maxInteractionDist = 500.0f;

	UPROPERTY(EditAnywhere)
		UCameraComponent* cameraComponent;

	UPROPERTY(EditAnywhere)
		ASwitch* currentSwitch;

	UPROPERTY(EditAnywhere)
		class USceneComponent* holdingComponent;
		
public:
	// Sets default values for this character's properties
	AFPCharacter();

	UPROPERTY(EditAnywhere)
		class AInteractableObject* currentItem;

	UPROPERTY(EditAnywhere)
		bool bCanMove;

	bool bHoldingItem;
	bool bInspecting;

	float pitchMax;
	float pitchMin;

	FVector holdingComp;
	FRotator lastRotation;

	FVector start;
	FVector forwardVec;
	FVector end;

	FHitResult hit;

	FComponentQueryParams defaultComponentQueryParams;
	FCollisionResponseParams defaultResponseParams;

protected:

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

	void OnInteract();
	void OnInspect();
	void OnInspectReleased();

	void ToggleMovement();
	void ToggleItemPickup();
};
