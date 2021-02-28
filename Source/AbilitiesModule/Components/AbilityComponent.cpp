// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "AbilitiesModule/AbilitiesModule.h"
#include "AbilitiesModule/Abilities/Ability.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UAbilityComponent::~UAbilityComponent()
{
}

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const int NumAbilities = Abilities.Num();
	for (int i = 0; i < NumAbilities; i++)
		Abilities[i]->Update(DeltaTime);

	const int NumEffects = Effects.Num();
	for (int i = 0; i < NumEffects; i++)
	{
		Effects[i]->Update(DeltaTime);

		if (Effects[i]->GetCurrentState() == EEffectState::EE_Finished)
		{
			Effects.RemoveAt(i);
			--i;
		}
	}
}

TArray<UAbility*>& UAbilityComponent::GetAbilities()
{
	return Abilities;
}

void UAbilityComponent::SetAbilities(const TArray<UAbility*>& NewAbilities)
{
	Abilities = NewAbilities;
}

UAbility* UAbilityComponent::PrepareAbility(const TSubclassOf<UAbility>& NewAbility, AActor* Owner)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbilityComponent::PrepareAbility"));

	UAbility* Ablitiy =  NewObject<UAbility>(this, NewAbility);
	Ablitiy->InitAbility(Owner);
	return Ablitiy;
}

void UAbilityComponent::AddAbility(UAbility* NewAbility)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbilityComponent::AddAbility"));

	Abilities.Add(NewAbility);
}

void UAbilityComponent::ExecuteAbility(UAbility* Ability)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbilityComponent::ExecuteAbility"));

	if (Ability->CanLaunchAbility())
	{
		//this one consume on release
		if (Ability->GetInputMode() != EAbilityInputMode::EI_ChargeAndRelease)
			Ability->ConsumeResources();

		Ability->RestartAbility();
	}
}

void UAbilityComponent::ExecuteEffect(UEffect* Effect)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("UAbilityComponent::ExecuteEffect"));

	Effect->RestartEffect(false);
	Effects.Add(Effect);
}