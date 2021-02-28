// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsComponent.h"
#include "UtilsModule/Libraries/UtilsLibrary.h"
#include "UtilsModule/UtilsTypes.h"
#include "StatsModule/StatTypes.h"
#include "Logging/LogMacros.h"
#include "StatsModule/StatsModule.h"
#include "../Interfaces/StatsActor.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	INIT_NUMSTAT_VALUES(ENumStats::ES_Life, Life)
	INIT_NUMSTAT_VALUES(ENumStats::ES_MaxLife, MaxLife)
	INIT_NUMSTAT_VALUES(ENumStats::ES_LifeRegenTime, LifeRegenTime)
	INIT_NUMSTAT_VALUES(ENumStats::ES_LifeRegenSpeed, LifeRegenSpeed)
	INIT_NUMSTAT_VALUES(ENumStats::ES_Mana, Mana)
	INIT_NUMSTAT_VALUES(ENumStats::ES_MaxMana, MaxMana)
	INIT_NUMSTAT_VALUES(ENumStats::ES_ManaRegenTime, ManaRegenTime)
	INIT_NUMSTAT_VALUES(ENumStats::ES_ManaRegenSpeed, ManaRegenSpeed)
	INIT_NUMSTAT_VALUES(ENumStats::ES_MoveSpeed, MoveSpeed)

}

// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Life regeneration
 	CurrentLifeRegenTime += DeltaTime;
 	if (CurrentLifeRegenTime >= LifeRegenTime.Value)
 	{
 		CurrentLifeRegenTime = LifeRegenTime.Value;
 		ModifyCurrentStat(Life, FNumericStatMod((LifeRegenSpeed.Value * DeltaTime), false));
 	}

	//Mana regeneration
	CurrentManaRegenTime += DeltaTime;
	if (CurrentManaRegenTime >= ManaRegenTime.Value)
	{
		CurrentManaRegenTime = ManaRegenTime.Value;
		ModifyCurrentStat(Mana, FNumericStatMod((ManaRegenSpeed.Value * DeltaTime), false));
	}
}

float UStatsComponent::GetCurrentLifeValue() const
{
	return Life.Value;
}

float UStatsComponent::GetCurrentMaxLifeValue() const
{
	return MaxLife.Value;
}

float UStatsComponent::GetCurrentManaValue() const
{
	return Mana.Value;
}

float UStatsComponent::GetCurrentMaxManaValue() const
{
	return MaxMana.Value;
}

void UStatsComponent::ModifyNumericStat(ENumStats Stat, const FNumericStatMod& Data)
{
	UE_LOG(StatsModuleLog, Display, TEXT("UStatsComponent::ModifyNumericStat"));

	FNumericStatValues& StatValues = GetNumStat(Stat);

 	const float OldValue = StatValues.Value;//previous value before update
 
 	//this not apply to stats like life or mana
 	if (!(CONDITION_CURRENT_STATS(Stat)))
 	{
 		Data.Percent ? StatValues.Multiplicitives += Data.Value : StatValues.Additives += Data.Value;
 
 		const float PercentValue = (StatValues.BaseValue + StatValues.Additives) * (StatValues.Multiplicitives / 100);
 		StatValues.Value = (StatValues.BaseValue + StatValues.Additives) + PercentValue;

		//check stats that cant be negative
		if (StatValues.Value < 0)
			StatValues.Value = 0;

		//update the stats that have max values when this ones change value
		if (CONDITION_MAX_STATS(Stat))
		{
			FNumericStatValues& CurrentStat = (StatValues.Stat == ENumStats::ES_MaxLife ? Life : Mana);
			const float CurrentValue = CurrentStat.Value;
			const float MaxValue = StatValues.Value;

			const float ModValue = ((OldValue > 0.0f) ? (CurrentValue * MaxValue / OldValue) - CurrentValue : CurrentValue);
			FNumericStatMod NewCurrentData(ModValue, false);

			ModifyNumericStat(CurrentStat.Stat, NewCurrentData);//update the current stat
		}
	}
	else //check stats that have max values
		ModifyCurrentStat(StatValues, Data);

	//call owner method to update local vars
	if (Cast<IStatsActor>(GetOwner()))
		IStatsActor::Execute_RefreshOwnerVars(GetOwner(), Stat);
}

float UStatsComponent::GetNumStatValue(ENumStats Stat)
{
	return GetNumStat(Stat).Value;
}

const FNumericStatValues& UStatsComponent::GetStat(ENumStats Stat)
{
	return GetNumStat(Stat);
}

FNumericStatValues& UStatsComponent::GetNumStat(ENumStats Stat)
{
	UE_LOG(StatsModuleLog, Display, TEXT("UStatsComponent::GetNumStat"));

	switch (Stat)
	{
	case ENumStats::ES_Life:			return Life; break;
	case ENumStats::ES_MaxLife:			return MaxLife; break;
	case ENumStats::ES_LifeRegenTime:	return LifeRegenTime; break;
	case ENumStats::ES_LifeRegenSpeed:	return LifeRegenSpeed; break;
	case ENumStats::ES_Mana:			return Mana; break;
	case ENumStats::ES_MaxMana:			return MaxMana; break;
	case ENumStats::ES_ManaRegenTime:	return ManaRegenTime; break;
	case ENumStats::ES_ManaRegenSpeed:	return ManaRegenSpeed; break;
	case ENumStats::ES_MoveSpeed:		return MoveSpeed; break;
	default:
		checkNoEntry();
		break;
	}

	return Life;
}

void UStatsComponent::ModifyCurrentStat(FNumericStatValues& CurrentStat, const FNumericStatMod& Data)
{
	UE_LOG(StatsModuleLog, Display, TEXT("UStatsComponent::ModifyCurrentStat"));

	if (Data.Value == 0)
		return;

	const float MaxValue = CurrentStat.Stat == ENumStats::ES_Life ? MaxLife.Value : MaxMana.Value;
	const float PreviousValue = CurrentStat.Value;

	//In this case the percent is over the Max Stat Value
	if (Data.Percent)
		CurrentStat.Value += MaxValue * (Data.Value / 100);
	else
		CurrentStat.Value += Data.Value;

	CurrentStat.Value = FMath::Clamp<float>(CurrentStat.Value, 0.0f, MaxValue);

	//Reset the timer for the regeneration stats
	CurrentStat.Stat == ENumStats::ES_Life && CurrentStat.Value < PreviousValue ? CurrentLifeRegenTime = 0 : CurrentLifeRegenTime = CurrentLifeRegenTime;
	CurrentStat.Stat == ENumStats::ES_Mana && CurrentStat.Value < PreviousValue ? CurrentManaRegenTime = 0 : CurrentManaRegenTime = CurrentManaRegenTime;
}