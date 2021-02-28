// Copyright Epic Games, Inc. All Rights Reserved.

#include "HMRHUD.h"
#include "HMR/HMR.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AHMRHUD::AHMRHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/HMR/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AHMRHUD::DrawHUD()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRHUD::DrawHUD"));

	Super::DrawHUD();
}
