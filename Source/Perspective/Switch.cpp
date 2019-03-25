// Fill out your copyright notice in the Description page of Project Settings.

#include "Switch.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ASwitch::ASwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isActive = false;

	visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	visibleComponent->SetSimulatePhysics(false);
	RootComponent = visibleComponent;
}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASwitch::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);

	if(isActive)
	{
		UMaterialInstanceDynamic* materialInstance_ = visibleComponent->CreateAndSetMaterialInstanceDynamic(0);
		if(materialInstance_)
		{
			materialInstance_->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
		}
	}
}
