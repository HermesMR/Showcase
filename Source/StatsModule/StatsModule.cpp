// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatsModule.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

IMPLEMENT_GAME_MODULE(FStatsModule, StatsModule);

DEFINE_LOG_CATEGORY(StatsModuleLog);

void FStatsModule::StartupModule()
{
	UE_LOG(StatsModuleLog, Display, TEXT("StartupModule StatsModule: Log Started"));
}

void FStatsModule::ShutdownModule()
{
	UE_LOG(StatsModuleLog, Display, TEXT("ShutdownModule StatsModule: Log Stopped"));
}