// Copyright Epic Games, Inc. All Rights Reserved.

#include "UtilsModule.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

IMPLEMENT_GAME_MODULE(FUtilsModule, UtilsModule);

DEFINE_LOG_CATEGORY(UtilsModuleLog);

void FUtilsModule::StartupModule()
{
	UE_LOG(UtilsModuleLog, Display, TEXT("StartupModule UtilsModule: Log Started"));
}

void FUtilsModule::ShutdownModule()
{
	UE_LOG(UtilsModuleLog, Display, TEXT("ShutdownModule UtilsModule: Log Stopped"));
}