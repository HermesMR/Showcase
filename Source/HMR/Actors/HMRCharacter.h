// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitiesModule/Abilities/Ability.h"
#include "AbilitiesModule/Interfaces/AbilityActor.h"
#include "StatsModule/Interfaces/StatsActor.h"
#include "Delegates/DelegateCombinations.h"
#include "HMRCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UStatsComponent;
class UAbilityComponent;
class UParticleSystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLeftHandAbilityChange, const TArray<UAbility*>&, LeftHandAbilities, int, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRightHandAbilityChange, const TArray<UAbility*>&, RightHandAbilities, int, Index);

UCLASS(config=Game)
class AHMRCharacter : public ACharacter, public IAbilityActor, public IStatsActor
{
	GENERATED_BODY()

public:

	AHMRCharacter();
	virtual ~AHMRCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HMR|Camera")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HMR|Camera")
	float BaseLookUpRate;

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const;

	const TArray<TSubclassOf<UAbility>>& GetLeftHandAbilitiesBP() const;
	const TArray<UAbility*>& GetLeftHandAbilities() const;
	UAbility* GetLeftHandSelectedAbility() const;

	const TArray<TSubclassOf<UAbility>>& GetRightHandAbilitiesBP() const;
	const TArray<UAbility*>& GetRightHandAbilities() const;
	UAbility* GetRightHandSelectedAbility() const;

	void PrepareAbilities();

	//AbilityActor Interface
	virtual UAbilityComponent* GetAbilityComponent_Implementation() const override;
	virtual UAbility* PrepareAbility_Implementation(TSubclassOf<UAbility> AbilityBP, AActor* AbilityOwner) override;
	virtual void AddAbility_Implementation(UAbility* Ability) override;
	virtual void LaunchAbility_Implementation(UAbility* Ability) override;
	virtual USkeletalMesh* GetSkeletalMesh_Implementation() const override;
	virtual USkeletalMeshComponent* GetSkeletalMeshComponent_Implementation() const override;
	virtual UCameraComponent* GetCameraComponent_Implementation() const override;
	virtual FRotator GetOwnerRotation_Implementation() const override;
	//End AbilityActor Interface
	
	//StatsActor Interface
	virtual UStatsComponent* GetStatsComponent_Implementation() const override;
	virtual void Heal_Implementation(float Value, bool Percent = false) override;
	virtual void ReceiveDamage_Implementation(float Value, bool Percent = false) override;
	virtual void RecoverMana_Implementation(float Value, bool Percent = false) override;
	virtual void ConsumeMana_Implementation(float Value, bool Percent = false) override;
	virtual void ModifyStat_Implementation(ENumStats Stat, float Value, bool Percent = false) override;
	virtual void ModifyMoveSpeed_Implementation(float Value, bool Percent = false) override;
	virtual void RefreshOwnerVars_Implementation(ENumStats Stat = ENumStats::ES_None) override;
	virtual void Kill_Implementation() override;
	virtual void OnDeath_Implementation() override;
	virtual void CheckLife_Implementation() override;
	//End StatsActor Interface
	
	UFUNCTION(BlueprintImplementableEvent, Category = "HMR|Stats")
	void CurrentLifeChange(bool Add);
	UFUNCTION(BlueprintImplementableEvent, Category = "HMR|Stats")
	void CurrentManaChange(bool Add);
	UFUNCTION(BlueprintImplementableEvent, Category = "HMR|Stats")
	void StatChange(ENumStats Stat, float NewValue, float OldValue, bool Add);
	UFUNCTION(BlueprintImplementableEvent, Category = "HMR|Stats")
	void MoveSpeedChange(bool Add);
	UFUNCTION(BlueprintImplementableEvent, Category = "HMR|Stats")
	void AbilityChange(bool Changed);

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);

	/** Left mouse input pressed*/
	void LeftHand();

	/** Left mouse input released*/
	void LeftHandReleased();

	/** Next Left Hand Ability input pressed*/
	void NextLeftHandAbility();

	/** Next Left Hand Ability input released*/
	void NextLeftHandAbilityReleased();

	/** Right mouse input pressed*/
	void RightHand();

	/** Right mouse input released*/
	void RightHandReleased();

	/** Next Right Hand Ability input pressed*/
	void NextRightHandAbility();

	/** Next Right Hand Ability input released*/
	void NextRightHandAbilityReleased();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HMR", meta = (AllowPrivateAccess = "true"))
	UStatsComponent* StatsComp;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HMR", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	//Abilities BP the character has in the left hand
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR", meta = (DisplayName = "Left Hand Abilities BP", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UAbility>> LeftHandAbilitiesBP;

	//Abilities the character has in the left hand
	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR", meta = (DisplayName = "Left Hand Abilities", AllowPrivateAccess = "true"))
	TArray<UAbility*> LeftHandAbilities;

	//Current selected ability in the left hand
	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR", meta = (AllowPrivateAccess = "true"))
	UAbility* LeftHandSelectedAbility;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR", meta = (DisplayName = "Left Hand Index", AllowPrivateAccess = "true"))
	int LeftHandIndex;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR", meta = (DisplayName = "LMB Pressed", AllowPrivateAccess = "true"))
	bool LMBPressed;

	//Abilities BP the character has in the right hand
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR", meta = (DisplayName = "Right Hand Abilities BP", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UAbility>> RightHandAbilitiesBP;

	//Abilities the character has in the Right hand
	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR", meta = (DisplayName = "Right Hand Abilities", AllowPrivateAccess = "true"))
	TArray<UAbility*> RightHandAbilities;

	//Current selected ability in the right hand
	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR", meta = (AllowPrivateAccess = "true"))
	UAbility* RightHandSelectedAbility;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR", meta = (DisplayName = "Right Hand Index", AllowPrivateAccess = "true"))
	int RightHandIndex;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "HMR", meta = (DisplayName = "RMB Pressed", AllowPrivateAccess = "true"))
	bool RMBPressed;

	UPROPERTY(BlueprintAssignable, Category = "HMR", meta = (DisplayName = "Left Hand Ability Change", AllowPrivateAccess = "true"))
	FLeftHandAbilityChange LeftHandAbilityChange;

	UPROPERTY(BlueprintAssignable, Category = "HMR", meta = (DisplayName = "Right Hand Ability Change", AllowPrivateAccess = "true"))
	FRightHandAbilityChange RightHandAbilityChange;

	void ChangeAbility(UAbility*& Ability, TArray<UAbility*>& Abilities, int& CurrentIndex);
};

