// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	mesh->SetSimulatePhysics(true);
	RootComponent = mesh;

	bHolding = false;
	bGravity = true;
	scaleFactor = this->RootComponent->GetComponentScale().X;
	originScale = this->GetActorScale();
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
}

void AInteractableObject::Init()
{
	MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (MyCharacter)
	{
		playerCam = MyCharacter->FindComponentByClass<UCameraComponent>();

		TArray<USceneComponent*> components;

		MyCharacter->GetComponents(components);

		if (components.Num() > 0)
		{
			for (auto& comp : components)
			{
				if (comp->GetName() == "HoldingComponent")
				{
					holdingComp = Cast<USceneComponent>(comp);
				}
			}
		}
	}
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!MyCharacter)
	{
		Init();
	}

	if(bHolding && holdingComp)
	{
		if (bInspecting)
		{
			SetActorLocationAndRotation(holdingComp->GetComponentLocation(), holdingComp->GetComponentRotation());
		}
		else
		{
			SetActorLocation(holdingComp->GetComponentLocation());
		}
	}
}

void AInteractableObject::RotateActor()
{
	controlRot = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	SetActorRotation(FQuat(controlRot));
}

void AInteractableObject::Pickup()
{
	bHolding = !bHolding;
	bGravity = !bGravity;
	mesh->SetEnableGravity(bGravity);
	mesh->SetSimulatePhysics(bHolding ? false : true);
	mesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if(!bHolding)
	{
		forwardVec = playerCam->GetForwardVector();
		SetOriginScale(this->GetRootComponent()->GetComponentScale());
	}
}

void AInteractableObject::SetInspecting(bool v_)
{
	bInspecting = v_;
}

void AInteractableObject::SetScaleFactor(const float scale_)
{
	scaleFactor = scale_;
}

void AInteractableObject::SetOriginScale(FVector oldScale_)
{
	originScale = oldScale_;
}

FVector AInteractableObject::GetOriginScale()
{
	return originScale;
}
