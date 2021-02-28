// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability.h"
#include "AbilitiesModule/AbilitiesModule.h"
#include "AbilitiesModule/Effects/SpawnActorEffect.h"
#include "StatsModule/Interfaces/StatsActor.h"
#include "StatsModule/Components/StatsComponent.h"
#include "AbilitiesModule/Effects/SearchAreaEffect.h"
#include "AbilitiesModule/Effects/ModifyStatEffect.h"
#include "AbilitiesModule/AbilityTypes.h"

UAbility::UAbility()
{
	CurrentState = EAbilityState::EA_Finished;
	CurrentCooldown = Cooldown;
	CurrentChargeTime = 0;
	AnimationID = EAnimationID::EA_Beam;
	Initialized = false;
	Blocked = false;
	OwnerStatsComp = nullptr;
}

UAbility::~UAbility()
{
}

void UAbility::InitAbility(AActor* NewOwner)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::InitAbility"));

	checkf(NewOwner && NewOwner->Implements<UAbilityActor>(), TEXT("The ability owner doesn't implement IAbilityActor Interface or is nullptr"))
	checkf(NewOwner && NewOwner->Implements<UStatsActor>(), TEXT("The ability owner doesn't implement IStatsActor Interface or is nullptr"))

	if (Initialized)
		return;

	OwnerActorAbility = NewOwner;
	OwnerActorStat = NewOwner;

	if (OwnerActorStat->Implements<UStatsActor>())
		OwnerStatsComp = IStatsActor::Execute_GetStatsComponent(OwnerActorStat);

	for (int i = 0; i < EffectsBP.Num(); i++)
	{
		const FEffectData& EffectData = EffectsBP[i];
		const EEffectType CurrentType = Cast<UEffect>(EffectData.Effect->GetDefaultObject())->GetEffectType();
		const EEffectTarget CurrentTarget = EffectData.Target;

		UEffect* NewEffect = nullptr;

		if (CurrentType == EEffectType::EE_SpawnActor)
			NewEffect = NewObject<USpawnActorEffect>(this, EffectsBP[i].Effect);
		else if (CurrentType == EEffectType::EE_SearchArea)
			NewEffect = NewObject<USearchAreaEffect>(this, EffectsBP[i].Effect);
		else if (CurrentType == EEffectType::EE_ModifyStat)
			NewEffect = NewObject<UModifyStatEffect>(this, EffectsBP[i].Effect);

		NewEffect->Init(NewOwner, this, CurrentTarget);
		Effects.Add(NewEffect);
	}

	Initialized = true;
}

bool UAbility::CanLaunchAbility_Implementation() const
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::CanLaunchAbility"));

	return HasResources() && CheckAdditionalConditions();
}

void UAbility::ConsumeResources_Implementation() const
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::ConsumeResources"));

	const float CurrentLife = OwnerStatsComp->GetLifeValue();
	const float MaxLife = OwnerStatsComp->GetMaxLifeValue();
	const float CurrentMana = OwnerStatsComp->GetManaValue();
	const float MaxMana = OwnerStatsComp->GetMaxManaValue();

	const float ConsumeLife = (LifeCostPercent ? MaxLife * (LifeCost / 100) : LifeCost);
	const float ConsumeMana = (ManaCostPercent ? MaxMana * (ManaCost / 100) : ManaCost);

	IStatsActor::Execute_ModifyStat(OwnerActorStat, ENumStats::ES_Life, -ConsumeLife, false);
	IStatsActor::Execute_ModifyStat(OwnerActorStat, ENumStats::ES_Mana, -ConsumeMana, false);
}

void UAbility::Update_Implementation(float ElapsedTime)
{
	if (CurrentState == EAbilityState::EA_Starting)
		StartAbility();
	if (CurrentState == EAbilityState::EA_Updating)
		UpdateAbility(ElapsedTime);
	if (CurrentState == EAbilityState::EA_Ending)
		EndAbility();
	if (CurrentState == EAbilityState::EA_Effects)
		LaunchEffects(ElapsedTime);
	if (CurrentState == EAbilityState::EA_Cooldown)
		UpdateCooldown(ElapsedTime);
}

void UAbility::RestartAbility_Implementation(bool Relaunch /*= false*/)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::RestartAbility"));

	CurrentState = EAbilityState::EA_Starting;
	CurrentCooldown = Cooldown;
	CurrentCastTime = 0;
	CurrentChargeTime = 0;

	for (int i = 0; i < Effects.Num(); i++)
		Effects[i]->RestartEffect(Relaunch);
}

bool UAbility::HasResources_Implementation() const
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::HasResources"));

	if (OwnerStatsComp)
	{
		const float CurrentLife = OwnerStatsComp->GetLifeValue();
		const float MaxLife = OwnerStatsComp->GetMaxLifeValue();
		const float CurrentMana = OwnerStatsComp->GetManaValue();
		const float MaxMana = OwnerStatsComp->GetMaxManaValue();

		const float ConsumeLife = (LifeCostPercent ? MaxLife * (LifeCost / 100) : LifeCost);
		const float ConsumeMana = (ManaCostPercent ? MaxMana * (ManaCost / 100) : ManaCost);

		return ConsumeLife <= CurrentLife && ConsumeMana <= CurrentMana;
	}

	return true;
}

bool UAbility::CheckAdditionalConditions_Implementation() const
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::CheckAdditionalConditions"));

	return !Blocked && CurrentState != EAbilityState::EA_Cooldown;
}

void UAbility::RelaunchAbility_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::RelaunchAbility"));

	RestartAbility(true);
	ConsumeResources();
	CurrentState = EAbilityState::EA_Effects;
	CurrentCastTime = CastTime;//no need to wait cast time when relaunch
}

void UAbility::StartAbility_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::StartAbility"));

	CurrentState = EAbilityState::EA_Updating;
}

void UAbility::UpdateAbility_Implementation(float ElapsedTime)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::UpdateAbility"));

	if (InputMode == EAbilityInputMode::EI_ChargeAndRelease && InputPressed)
	{
		//only when start
		if (CurrentChargeTime == 0.0f)
			StartCharging.Broadcast(this);

		CurrentChargeTime += ElapsedTime;
	}
	else if (InputMode == EAbilityInputMode::EI_ChargeAndRelease && !InputPressed && CurrentChargeTime < ChargeTime)
		CancelAbility();

	if (InputMode == EAbilityInputMode::EI_OneHit
		|| (InputMode == EAbilityInputMode::EI_ChargeAndRelease && !InputPressed && CurrentChargeTime >= ChargeTime))
	{
		CurrentCastTime += ElapsedTime;

		if (CurrentCastTime >= CastTime)
		{
			if (InputMode == EAbilityInputMode::EI_ChargeAndRelease)
			{
				ConsumeResources();
				ReleaseCharging.Broadcast(this);
			}
			
			CurrentState = EAbilityState::EA_Ending;
		}
	}
}

void UAbility::EndAbility_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::EndAbility"));

	CurrentState = EAbilityState::EA_Effects;
}

void UAbility::LaunchEffects_Implementation(float ElapsedTime)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::LaunchEffects"));

	bool EffectsFinished = true;
	for (int i = 0; i < Effects.Num(); i++)
	{
		Effects[i]->Update(ElapsedTime);
		if (Effects[i]->GetCurrentState() != EEffectState::EE_Finished)
			EffectsFinished = false;
	}

	if (EffectsFinished)
	{
		CurrentState = EAbilityState::EA_Cooldown;
		StartCooldown.Broadcast(this);
	}
}

void UAbility::UpdateCooldown_Implementation(float ElapsedTime)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::UpdateCooldown"));

	CurrentCooldown -= ElapsedTime;

	if (CurrentCooldown <= 0)
	{
		CurrentState = EAbilityState::EA_CooldownEnd;

		if (CanHoldDown && InputPressed && CanLaunchAbility())
			RelaunchAbility();
		else
		{
			SetInputPressed(false);//at this point no need this value for the ability but need int for the effects
			for (int i = 0; i < Effects.Num(); i++)
				Effects[i]->CancelEffect();

			CurrentState = EAbilityState::EA_Finished;
		}
	}
}

bool UAbility::GetInputPressed() const
{
	return InputPressed;
}

void UAbility::SetInputPressed(bool NewInputPressed)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::SetInputPressed"));

	if (InputMode == EAbilityInputMode::EI_ChargeAndRelease)
		CancelCharging.Broadcast(this);

	InputPressed = NewInputPressed;

	for (int i = 0; i < Effects.Num(); i++)
		Effects[i]->SetInputPressed(InputPressed);
}

void UAbility::CancelAbility_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbility::CancelAbility"));

	CurrentState = EAbilityState::EA_Finished;
	InputPressed = false;
}

bool UAbility::IsRunning() const
{
	return CurrentState != EAbilityState::EA_Finished && CurrentState != EAbilityState::EA_Cooldown;
}

bool UAbility::IsCooldown() const
{
	return CurrentState == EAbilityState::EA_Cooldown;
}