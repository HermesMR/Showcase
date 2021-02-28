// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect.h"
#include "AbilitiesModule/AbilitiesModule.h"

UEffect::UEffect()
{
	EffectType = EEffectType::EE_None;
	CurrentState = EEffectState::EE_Starting;
	OwnerActor = nullptr;
	OwnerAbility = nullptr;
	InputPressed = false;
	Relaunched = false;
	TargetActor = nullptr;
	TargetPoint = FVector::ZeroVector;
	TargetType = EEffectTarget::EE_None;
}

UEffect::~UEffect()
{
}

void UEffect::Init(AActor* NewOwner, UAbility* NewOwnerAbility /*= nullptr*/, EEffectTarget NewTarget /*= EEffectTarget::EE_Owner*/)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UEffect::Init"));

	checkf(NewOwner && NewOwner->Implements<UAbilityActor>(), TEXT("The ability owner doesn't implement IAbilityActor Interface or is nullptr"))

	OwnerActor = NewOwner;
	OwnerAbility = NewOwnerAbility;
	TargetType = NewTarget;
}

void UEffect::Update_Implementation(float ElapsedTime)
{
	if (CurrentState == EEffectState::EE_Starting)
		StartEffect();
	if (CurrentState == EEffectState::EE_Updating)
		UpdateEffect(ElapsedTime);
	if (CurrentState == EEffectState::EE_Ending)
		EndEffect();
}

void UEffect::RestartEffect_Implementation(bool Relaunch /*= false*/)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UEffect::RestartEffect"));

	CurrentState = EEffectState::EE_Starting;
	TargetActor = nullptr;
	TargetPoint = FVector::ZeroVector;
}

void UEffect::CancelEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UEffect::CancelEffect"));

	EndEffect();
}

void UEffect::StartEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UEffect::StartEffect"));

	CurrentState = EEffectState::EE_Updating;
}

void UEffect::UpdateEffect_Implementation(float ElapsedTime)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UEffect::UpdateEffect"));

	CurrentState = EEffectState::EE_Ending;
}

void UEffect::EndEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UEffect::EndEffect"));

	CurrentState = EEffectState::EE_Finished;
}
