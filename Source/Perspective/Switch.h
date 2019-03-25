// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Switch.generated.h"


UCLASS()
class PERSPECTIVE_API ASwitch : public AActor
{
GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool* isActive;

public:	
	// Called every frame
	virtual void Tick(float deltaTime_) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* visibleComponent;

	bool GetActiveState() const
	{
		return isActive;
	}
	void SetActiveState(bool* activeState_)
	{
		isActive = activeState_;
	}
};
