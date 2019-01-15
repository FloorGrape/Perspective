// Written By Joseph Ariganello

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 *
 */
UCLASS()
class PERSPECTIVE_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:

	AMyHUD(const FObjectInitializer& objectInitializer_);

	virtual void DrawHUD() override;

private:
	UTexture2D* crossHair;

};
