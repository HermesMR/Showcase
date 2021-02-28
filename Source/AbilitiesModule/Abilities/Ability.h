// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitiesModule/AbilityTypes.h"
#include "AbilitiesModule/Effects/Effect.h"
#include "Delegates/DelegateCombinations.h"
#include "UtilsModule/UtilsTypes.h"
#include "Ability.generated.h"

class UParticleSystem;
class IAbilityActor;
class IStatsActor;
class UStatsComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartCooldown, UAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartCharging, UAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCancelCharging, UAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReleaseCharging, UAbility*, Ability);

UENUM(BlueprintType)
enum class EAbilityState : uint8
{
	EA_None							UMETA(DisplayName = "None", Hidden),
	EA_Starting						UMETA(DisplayName = "Starting"),
	EA_Updating						UMETA(DisplayName = "Updating"),
	EA_Ending						UMETA(DisplayName = "Ending"),
	EA_Effects						UMETA(DisplayName = "Effects"),
	EA_Finished						UMETA(DisplayName = "Finished"),
	EA_Cooldown						UMETA(DisplayName = "Cooldown"),
	EA_CooldownEnd					UMETA(DisplayName = "CooldownEnd")
};

/**
 *
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "HMR Ability")
class ABILITIESMODULE_API UAbility : public UObject
{
	GENERATED_BODY()

public:
	UAbility();
	~UAbility();

	void InitAbility(AActor* NewOwner);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Can Launch Ability"))
	bool CanLaunchAbility() const;
	bool CanLaunchAbility_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Consume Resources"))
	void ConsumeResources() const;
	void ConsumeResources_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Update"))
	void Update(float ElapsedTime);
	void Update_Implementation(float ElapsedTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Restart Ability"))
	void RestartAbility(bool Relaunch = false);
	void RestartAbility_Implementation(bool Relaunch = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Cancel Ability"))
	void CancelAbility();
	void CancelAbility_Implementation();

	UFUNCTION(BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Is Running"))
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Is Cooldown"))
	bool IsCooldown() const;
	
	VAR_GET_SET(FText, Name)
	VAR_GET_SET(UObject*, AbilityIMG)
	VAR_GET_SET(float, ManaCost)
	VAR_GET_SET(bool, ManaCostPercent)
	VAR_GET_SET(float, LifeCost)
	VAR_GET_SET(bool, LifeCostPercent)
	VAR_GET_SET(float, Cooldown)
	VAR_GET_SET(float, CurrentCooldown)
	VAR_GET_SET(float, CastTime)
	VAR_GET_SET(float, CurrentCastTime)
	VAR_GET_SET(EAbilityInputMode, InputMode)
	VAR_GET_SET(bool, CanHoldDown)
	VAR_GET_SET(EAbilityState, CurrentState)
	VAR_GET_SET(bool, Blocked)
	VAR_GET_SET(EAnimationID, AnimationID)
	VAR_GET_SET(bool, Initialized)

	bool GetInputPressed() const;
	void SetInputPressed(bool NewInputPressed);

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Has Resources"))
	bool HasResources() const;
	bool HasResources_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Check Additional Conditions"))
	bool CheckAdditionalConditions() const;
	bool CheckAdditionalConditions_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Relaunch Ability"))
	void RelaunchAbility();
	void RelaunchAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Start Ability"))
	void StartAbility();
	void StartAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Update Ability"))
	void UpdateAbility(float ElapsedTime);
	void UpdateAbility_Implementation(float ElapsedTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "End Ability"))
	void EndAbility();
	void EndAbility_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Launch Effects"))
	void LaunchEffects(float ElapsedTime);
	void LaunchEffects_Implementation(float ElapsedTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HMR Ability", meta = (DisplayName = "Update Cooldown"))
	void UpdateCooldown(float ElapsedTime);
	void UpdateCooldown_Implementation(float ElapsedTime);

	//Call it when the cool down start
	UPROPERTY(BlueprintAssignable, Category = "HMR|Ability", meta = (DisplayName = "Start Cooldown", AllowPrivateAccess = "true"))
	FStartCooldown StartCooldown;

	//Call it when start to charge the ability
	UPROPERTY(BlueprintAssignable, Category = "HMR|Ability", meta = (DisplayName = "Start Charging", AllowPrivateAccess = "true"))
	FStartCharging StartCharging;

	//Call it when release a charge ability
	UPROPERTY(BlueprintAssignable, Category = "HMR|Ability", meta = (DisplayName = "Release Charging", AllowPrivateAccess = "true"))
	FReleaseCharging ReleaseCharging;

	//Call it when cancell a charging ability
	UPROPERTY(BlueprintAssignable, Category = "HMR|Ability", meta = (DisplayName = "Cancel Charging", AllowPrivateAccess = "true"))
	FCancelCharging CancelCharging;

	//Ability name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Name", AllowPrivateAccess = "true"))
	FText Name;

	/**
	 * Ability UI Img.
	 * The image to render for this brush, can be a UTexture or UMaterialInterface or an object implementing
	 * the AtlasedTextureInterface.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true", DisplayName = "Ability UI Img", AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* AbilityIMG;

	//Mana cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Mana Cost", UIMin = 0.0f, AllowPrivateAccess = "true"))
	float ManaCost;

	//If the cost of the mana is going to be percentage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Mana Cost Percent", AllowPrivateAccess = "true"))
	bool ManaCostPercent;

	//Life cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Life Cost", UIMin = 0.0f, AllowPrivateAccess = "true"))
	float LifeCost;

	//If the cost of the life is going to be percentage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Life Cost Percent", AllowPrivateAccess = "true"))
	bool LifeCostPercent;

	//Ability cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Cooldown", UIMin = 0.0f, AllowPrivateAccess = "true"))
	float Cooldown;

	UPROPERTY(BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Current Cooldown", AllowPrivateAccess = "true"))
	float CurrentCooldown;

	//If the cost of the life is going to be percentage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Cast Time", UIMin = 0.0f, AllowPrivateAccess = "true"))
	float CastTime;

	UPROPERTY(BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Current Cast Time", AllowPrivateAccess = "true"))
	float CurrentCastTime;

	//Set the input mode to launch the ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Ability Input", AllowPrivateAccess = "true"))
	EAbilityInputMode InputMode;

	//If the input mode is "OneHit" you can hold the input and continue spawning the effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Can Hold Down", EditCondition = "InputMode == EAbilityInputMode::EI_OneHit", AllowPrivateAccess = "true"))
	bool CanHoldDown;

	//Time to charge the ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Charge Time", EditCondition = "InputMode == EAbilityInputMode::EI_ChargeAndRelease", AllowPrivateAccess = "true"))
	float ChargeTime;

	UPROPERTY(BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Current Charge Time", AllowPrivateAccess = "true"))
	float CurrentChargeTime;

	//Ability to play when ability launch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Animation ID", AllowPrivateAccess = "true"))
	EAnimationID AnimationID;

	//BP of the effects the ability is going to launch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Effects To Launch", AllowPrivateAccess = "true"))
	TArray<FEffectData> EffectsBP;

	//C++ effects objects the ability is going to launch
	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR Ability", meta = (DisplayName = "Effects", AllowPrivateAccess = "true"))
	TArray<UEffect*> Effects;

	//Effect current state
	UPROPERTY(BlueprintReadOnly, Category = "HMR Ability", meta = (DisplayName = "CurrentState", AllowPrivateAccess = "true"))
	EAbilityState CurrentState;

	//Actor that has the Ability interface
	UPROPERTY(BlueprintReadOnly, Category = "HMR Ability", meta = (DisplayName = "Ability Owner", AllowPrivateAccess = "true"))
	AActor* OwnerActorAbility;

	//Actor that has the Stats interface
	UPROPERTY(BlueprintReadOnly, Category = "HMR Ability", meta = (DisplayName = "Stat Owner", AllowPrivateAccess = "true"))
	AActor*  OwnerActorStat;

	//Stats component from the actor
	UPROPERTY(BlueprintReadOnly, Category = "HMR Ability", meta = (DisplayName = "Owner Stats Component", AllowPrivateAccess = "true"))
	UStatsComponent* OwnerStatsComp;

	//Checks if the owner has the input pressed
	UPROPERTY(BlueprintReadOnly, Category = "HMR Ability", meta = (DisplayName = "Input Pressed", AllowPrivateAccess = "true"))
	bool InputPressed;

	UPROPERTY(BlueprintReadOnly, Category = "HMR Ability", meta = (DisplayName = "Initialized", AllowPrivateAccess = "true"))
	bool Initialized;

	//Ability blocked state
	UPROPERTY(BlueprintReadOnly, Category = "HMR Ability", meta = (DisplayName = "Initialized", AllowPrivateAccess = "true"))
	bool Blocked;
};