// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitiesModule/Interfaces/AbilityActor.h"
#include "UtilsModule/UtilsTypes.h"
#include "AbilitiesModule/AbilityTypes.h"
#include "Effect.generated.h"

class IAbilityActor;
class UAbility;

UENUM(BlueprintType)
enum class EEffectState : uint8
{
	EE_None							UMETA(DisplayName = "None", Hidden),
	EE_Starting						UMETA(DisplayName = "Starting"),
	EE_Updating						UMETA(DisplayName = "Updating"),
	EE_Ending						UMETA(DisplayName = "Ending"),
	EE_Finished						UMETA(DisplayName = "Finished")
};

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	EE_None							UMETA(DisplayName = "None", Hidden),
	EE_ModifyStat					UMETA(DisplayName = "Modify Stat"),
	EE_SearchArea					UMETA(DisplayName = "Search Area"),
	EE_SpawnActor					UMETA(DisplayName = "Spawn Actor"),
	EE_ApplyBehaviour				UMETA(DisplayName = "Apply Behaviour")//not implemented
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "Effect")
class ABILITIESMODULE_API UEffect : public UObject
{
	GENERATED_BODY()

public:
	UEffect();
	virtual ~UEffect();

	virtual void Init(AActor* NewOwner, UAbility* NewOwnerAbility = nullptr, EEffectTarget NewTarget = EEffectTarget::EE_Owner);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Effect", meta = (DisplayName = "Update"))
	void Update(float ElapsedTime);
	virtual void Update_Implementation(float ElapsedTime);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Effect", meta = (DisplayName = "Restart Effect"))
	void RestartEffect(bool Relaunch = false);
	virtual void RestartEffect_Implementation(bool Relaunch = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Effect", meta = (DisplayName = "Cancel Effect"))
	void CancelEffect();
	virtual void CancelEffect_Implementation();

	VAR_GET_SET(FText, Name)
	VAR_GET_SET(AActor*, OwnerActor)
	VAR_GET_SET(UAbility*, OwnerAbility)
	VAR_GET_SET(EEffectType, EffectType)
	VAR_GET_SET(EEffectState, CurrentState)
	VAR_GET_SET(bool, InputPressed)
	VAR_GET_SET(bool, Relaunched)
	VAR_GET_SET(AActor*, TargetActor)
	VAR_GET_SET(FVector, TargetPoint)
	VAR_GET_SET(EEffectTarget, TargetType)

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Effect", meta = (DisplayName = "Start Effect"))
	void StartEffect();
	virtual void StartEffect_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Effect", meta = (DisplayName = "Update Effect"))
	void UpdateEffect(float ElapsedTime);
	virtual void UpdateEffect_Implementation(float ElapsedTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Effect", meta = (DisplayName = "End Effect"))
	void EndEffect();
	virtual void EndEffect_Implementation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect", meta = (DisplayName = "Effect Name", AllowPrivateAccess = "true"))
	FText Name;

	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect", meta = (DisplayName = "Input Pressed", AllowPrivateAccess = "true"))
	bool InputPressed;

	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect", meta = (DisplayName = "Relaunched", AllowPrivateAccess = "true"))
	bool Relaunched;

	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect", meta = (DisplayName = "Effect Type", AllowPrivateAccess = "true"))
	EEffectType EffectType;

	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect", meta = (DisplayName = "Current State", AllowPrivateAccess = "true"))
	EEffectState CurrentState;

	//this refer to the actor that implements the IAbilityInterface
	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect", meta = (DisplayName = "Owner Actor Ability", AllowPrivateAccess = "true"))
	AActor* OwnerActor;

	//Ability that launch the effect
	UPROPERTY(BlueprintReadWrite, Transient, Category = "HMR|Effect", meta = (DisplayName = "Owner Ability", AllowPrivateAccess = "true"))
	UAbility* OwnerAbility;

	//Actor target
	UPROPERTY(BlueprintReadWrite, Transient, Category = "HMR|Effect", meta = (DisplayName = "Target Actor", AllowPrivateAccess = "true"))
	AActor* TargetActor;

	//Ground target
	UPROPERTY(BlueprintReadWrite, Transient, Category = "HMR|Effect", meta = (DisplayName = "Target Point", AllowPrivateAccess = "true"))
	FVector TargetPoint;

	//Target for the effect
	UPROPERTY(BlueprintReadWrite, Transient, Category = "HMR|Effect", meta = (DisplayName = "Target", AllowPrivateAccess = "true"))
	EEffectTarget TargetType;
};
