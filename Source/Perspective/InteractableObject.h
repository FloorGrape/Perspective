// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "InteractableObject.generated.h"

class ACharacter;

UCLASS()
class PERSPECTIVE_API AInteractableObject : public AActor
{
GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Init();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* holdingComp;

	UFUNCTION()
	void RotateActor();

	UFUNCTION()
	void Pickup();

	void SetInspecting(bool v_);

	bool bHolding;
	bool bInspecting;
	bool bGravity;

	FRotator controlRot;
	FVector forwardVec;
	ACharacter* MyCharacter;
	UCameraComponent* playerCam;

	UPROPERTY(VisibleAnywhere)
		float scaleFactor;

	UFUNCTION()
		void SetScaleFactor(const float scale_);

	UPROPERTY(VisibleAnywhere)
		FVector originScale;
};
