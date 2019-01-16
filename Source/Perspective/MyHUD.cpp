// Written By Joseph Ariganello
#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AMyHUD::AMyHUD(const FObjectInitializer& objectInitializer_) : Super(objectInitializer_)
{
	//Find the crosshair texture.
	static ConstructorHelpers::FObjectFinder<UTexture2D> crossHairObj(TEXT("Texture2D'/Game/Assets/crosshair.crosshair'"));

	//Set the crosshair texture to the texture found.
	crossHair = crossHairObj.Object;
}

void AMyHUD::DrawHUD() {
	Super::DrawHUD();

	//Determine the Center of the canvas, which happens to be the screen size.
	const FVector2D center(Canvas->ClipX * 0.5, Canvas->ClipY * 0.5);

	//Must offset by half of the texture's dimensions so that it will allign at the center.
	const FVector2D crosshairDrawPosition((center.X - (crossHair->GetSurfaceWidth() * 0.5)), (center.Y - (crossHair->GetSurfaceHeight() * 0.5)));

	//Create a tile item for the canvas and give it the crosshair position and texture.
	FCanvasTileItem TileItem(crosshairDrawPosition, crossHair->Resource, FLinearColor::White);

	//This will make the crosshair more translucent when it is being drawn over something.
	TileItem.BlendMode = SE_BLEND_Translucent;

	//Draw the tile item.
	Canvas->DrawItem(TileItem);
}