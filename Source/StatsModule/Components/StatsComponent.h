// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsModule/StatTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "StatsComponent.generated.h"

class IStatsActor;

#define INIT_NUMSTAT_VALUES(CurrentStat, StatName) \
				StatName.Stat = CurrentStat; \
				StatName.Value = StatName.BaseValue;


#define STAT_FUNCTIONALITY(Type, Name) \
				STAT_GET_STRUCT(Name) \
				STAT_GET(Type, Name) \
				STAT_SET(Type, Name) \
				STAT_MODIFY(Type, Name)

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STATSMODULE_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "HMR Stats")
	float GetCurrentLifeValue() const;

	UFUNCTION(BlueprintCallable, Category = "HMR Stats")
	float GetCurrentMaxLifeValue() const;

	UFUNCTION(BlueprintCallable, Category = "HMR Stats")
	float GetCurrentManaValue() const;

	UFUNCTION(BlueprintCallable, Category = "HMR Stats")
	float GetCurrentMaxManaValue() const;

	UFUNCTION(BlueprintCallable, Category = "HMR Stats")
	void ModifyNumericStat(ENumStats Stat, const FNumericStatMod& Data);

	UFUNCTION(BlueprintCallable, Category = "HMR Stats")
	const FNumericStatValues& GetStat(ENumStats Stat);

	UFUNCTION(BlueprintCallable, Category = "HMR Stats")
	float GetNumStatValue(ENumStats Stat);

	STAT_FUNCTIONALITY(float, Life)
	STAT_FUNCTIONALITY(float, MaxLife)
	STAT_FUNCTIONALITY(float, LifeRegenTime)
	STAT_FUNCTIONALITY(float, LifeRegenSpeed)
	STAT_FUNCTIONALITY(float, Mana)
	STAT_FUNCTIONALITY(float, MaxMana)
	STAT_FUNCTIONALITY(float, ManaRegenTime)
	STAT_FUNCTIONALITY(float, ManaRegenSpeed)
	STAT_FUNCTIONALITY(float, MoveSpeed)

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	FNumericStatValues& GetNumStat(ENumStats Stat);
	void ModifyCurrentStat(FNumericStatValues& CurrentStat, const FNumericStatMod& Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Life", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues Life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Max Life", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues MaxLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Life Regeneration Time", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues LifeRegenTime;

	UPROPERTY(BlueprintReadOnly, Category = "HMR Stats", meta = (DisplayName = "Current Life Regeneration Time", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	float CurrentLifeRegenTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Life Regeneration Speed", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues LifeRegenSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Mana", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "MaxMana", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Mana Regeneration Time", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues ManaRegenTime;

	UPROPERTY(BlueprintReadOnly, Category = "HMR Stats", meta = (DisplayName = "Current Mana Regeneration Time", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	float CurrentManaRegenTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Mana Regeneration Speed", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues ManaRegenSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "MoveSpeed", AllowPrivateAccess = "true", ShowOnlyInnerProperties))
	FNumericStatValues MoveSpeed;
};
