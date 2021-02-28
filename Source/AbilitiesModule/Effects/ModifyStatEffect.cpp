// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesModule/Effects/ModifyStatEffect.h"
#include "StatsModule/Interfaces/StatsActor.h"
#include "AbilitiesModule/AbilitiesModule.h"

UModifyStatEffect::UModifyStatEffect()
{
	EffectType = EEffectType::EE_ModifyStat;
	Stat = ENumStats::ES_Life;
	Value = 0.0f;
	Percent = false;
}

UModifyStatEffect::~UModifyStatEffect()
{
}

void UModifyStatEffect::StartEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UModifyStatEffect::StartEffect"));

	//If targettype is "Target" will have to be set it before effect launch
	if (TargetType == EEffectTarget::EE_Owner)
		TargetActor = OwnerActor;

	CurrentState = EEffectState::EE_Updating;
}

void UModifyStatEffect::UpdateEffect_Implementation(float ElapsedTime)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UModifyStatEffect::UpdateEffect"));

	if (TargetActor->Implements<UStatsActor>())//check interface
	{
		IStatsActor::Execute_ModifyStat(TargetActor, Stat, Value, Percent);
	}

	CurrentState = EEffectState::EE_Ending;
}

void UModifyStatEffect::EndEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UModifyStatEffect::EndEffect"));

	CurrentState = EEffectState::EE_Finished;
}