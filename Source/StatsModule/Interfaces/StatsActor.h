// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatsModule/StatTypes.h"
#include "StatsActor.generated.h"

class UStatsComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UStatsActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STATSMODULE_API IStatsActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Get Stats Component"))
	UStatsComponent* GetStatsComponent() const;
	virtual UStatsComponent* GetStatsComponent_Implementation() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Heal"))
	void Heal(float Value, bool Percent = false);
	virtual void Heal_Implementation(float Value, bool Percent = false) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Receive Damage"))
	void ReceiveDamage(float Value, bool Percent = false);
	virtual void ReceiveDamage_Implementation(float Value, bool Percent = false) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Recover Mana"))
	void RecoverMana(float Value, bool Percent = false);
	virtual void RecoverMana_Implementation(float Value, bool Percent = false) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Consume Mana"))
	void ConsumeMana(float Value, bool Percent = false);
	virtual void ConsumeMana_Implementation(float Value, bool Percent = false) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Modify Stat"))
	void ModifyStat(ENumStats Stat, float Value, bool Percent = false);
	virtual void ModifyStat_Implementation(ENumStats Stat, float Value, bool Percent = false) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Modify Move Speed"))
	void ModifyMoveSpeed(float Value, bool Percent = false);
	virtual void ModifyMoveSpeed_Implementation(float Value, bool Percent = false) = 0;

	/**
	 * @fn	virtual void IStatsActor::RefreshOwnerVars(ENumStats Stat) = 0;
	 *
	 * @brief	Method to update or change local actor vars that are related with the interface stats
	 *
	 * @param 	Stat to update, if stat == NONE will update all
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Refresh Owner Vars"))
	void RefreshOwnerVars(ENumStats Stat);
	virtual void RefreshOwnerVars_Implementation(ENumStats Stat = ENumStats::ES_None) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Kill"))
	void Kill();
	virtual void Kill_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::On Death"))
	void OnDeath();
	virtual void OnDeath_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IStatsActor::Check Life"))
	void CheckLife();
	virtual void CheckLife_Implementation() = 0;
};
