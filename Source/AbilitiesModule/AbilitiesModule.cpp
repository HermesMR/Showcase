// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitiesModule.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

IMPLEMENT_GAME_MODULE(FAbilitiesModule, AbilitiesModule);

DEFINE_LOG_CATEGORY(AbilitiesModuleLog);

void FAbilitiesModule::StartupModule()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("StartupModule AbilitiesModule: Log Started"));
}

void FAbilitiesModule::ShutdownModule()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("ShutdownModule AbilitiesModule: Log Stopped"));
}