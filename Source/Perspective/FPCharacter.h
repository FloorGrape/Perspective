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

	UPROPERTY(VisibleAnywhere)
		float maxWalkSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere)
		float sprintModifier = 3.0f;

	UPROPERTY(VisibleAnywhere)
		float maxInteractionDist = 10000.0f;

	UPROPERTY(EditAnywhere)
		UCameraComponent* cameraComponent;

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

	UPROPERTY(VisibleAnywhere)
		FVector forwardVec;

	UPROPERTY(VisibleAnywhere)
		float distance;

	float angularSize;

	FVector start;
	FVector end;
	FHitResult hit;

	FCollisionQueryParams defaultCollisionQueryParams;
	FCollisionQueryParams holdingCollisionQueryParams;
	FCollisionResponseParams defaultResponseParams;

	float itemScaleFactor;

	// attempt



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

	void OnInteract();
	void OnInspect();
	void OnInspectReleased();

	void ToggleMovement();
	void ToggleItemPickup();
	void OnForcePerspective(AInteractableObject* object_, const float angSize_, const float prevSize_, FHitResult result_);
	FVector GroundCheck(FHitResult ht_);
	float AngularSize(const AInteractableObject* object_) const;
	float ActualSize(const float angSize_, const float dist_) const;
	float Distance(const FVector posA_, const FVector posB_) const;
};
