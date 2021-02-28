// Copyright Epic Games, Inc. All Rights Reserved.

#include "HMRGameMode.h"
#include "HMRHUD.h"
#include "HMR/Actors/HMRCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHMRGameMode::AHMRGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/HMR/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AHMRHUD::StaticClass();
}
