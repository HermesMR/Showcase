// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitiesModule/Effects/Effect.h"
#include "SearchAreaEffect.generated.h"

//Id to search for actors or ground points
UENUM(BlueprintType)
enum class ESearchTarget : uint8
{
	ES_None							UMETA(DisplayName = "None", Hidden),
	ES_Actors						UMETA(DisplayName = "Actors"),
	ES_GroundPoints					UMETA(DisplayName = "Ground Points")//not implemented
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "Search Area Effect")
class ABILITIESMODULE_API USearchAreaEffect : public UEffect
{
	GENERATED_BODY()
	

public:
	USearchAreaEffect();
	~USearchAreaEffect();

	virtual void Init(AActor* NewOwner, UAbility* NewOwnerAbility = nullptr, EEffectTarget NewTarget = EEffectTarget::EE_Owner) override;
	virtual void RestartEffect_Implementation(bool Relaunch = false) override;
	void PrepareSubEffects();

protected:
	virtual void StartEffect_Implementation() override;
	virtual void UpdateEffect_Implementation(float ElapsedTime) override;
	virtual void EndEffect_Implementation() override;

private:
	//Blueprint effects that will be launched to the targets of the search area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Effects BP", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UEffect>> SubEffectsBP;

	//C++ effects objects that will be launched to the targets of the search area
	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Effects", AllowPrivateAccess = "true"))
	TArray<UEffect*> SubEffects;

	//Radius of the area to search
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Radius", UIMin = 0.0f, AllowPrivateAccess = "true"))
	float Radius;

	//Type of target to search
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Type Of Target", AllowPrivateAccess = "true"))
	ESearchTarget SearchTarget;

	//If true will apply the effects to all targets found
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Apply To All Targets", AllowPrivateAccess = "true"))
	bool ApplyAllTargets;

	//Max number of targets will apply the effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Max Number Targets", EditCondition = "!ApplyAllTargets", UIMin = 1, AllowPrivateAccess = "true"))
	int MaxNumTargets;

	//Spawn Location
	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Spawn Location", AllowPrivateAccess = "true"))
	FVector SpawnLocation;

	//Actor targets array
	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Actor Targets", AllowPrivateAccess = "true"))
	TArray<AActor*> ActorTargets;

	//Point targets array
	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect|Search Area", meta = (DisplayName = "Point Targets", AllowPrivateAccess = "true"))
	TArray<FVector> PointTargets;
};
