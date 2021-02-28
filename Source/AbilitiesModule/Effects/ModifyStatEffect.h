// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitiesModule/Effects/Effect.h"
#include <StatsModule/StatTypes.h>
#include "ModifyStatEffect.generated.h"

class IStatsActor;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "Modify Stat Effect")
class ABILITIESMODULE_API UModifyStatEffect : public UEffect
{
	GENERATED_BODY()

public:
	UModifyStatEffect();
	~UModifyStatEffect();

protected:
	virtual void StartEffect_Implementation() override;
	virtual void UpdateEffect_Implementation(float ElapsedTime) override;
	virtual void EndEffect_Implementation() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Modify Stat", meta = (DisplayName = "Stat", AllowPrivateAccess = "true"))
	ENumStats Stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Modify Stat", meta = (DisplayName = "Value", AllowPrivateAccess = "true"))
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Modify Stat", meta = (DisplayName = "Percent", AllowPrivateAccess = "true"))
	bool Percent;
};
