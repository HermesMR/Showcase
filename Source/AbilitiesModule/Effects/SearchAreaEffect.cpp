// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesModule/Effects/SearchAreaEffect.h"
#include "AbilitiesModule/Effects/SpawnActorEffect.h"
#include "AbilitiesModule/Effects/ModifyStatEffect.h"
#include "AbilitiesModule/Interfaces/AbilityActor.h"
#include "AbilitiesModule/AbilitiesModule.h"
#include "StatsModule/Interfaces/StatsActor.h"
#include <DrawDebugHelpers.h>
#include "HMR/HMRTypes.h"

USearchAreaEffect::USearchAreaEffect()
{
	EffectType = EEffectType::EE_SearchArea;
	SearchTarget = ESearchTarget::ES_Actors;
	ApplyAllTargets = true;
	MaxNumTargets = 1;
	SpawnLocation = FVector::ZeroVector;
}

USearchAreaEffect::~USearchAreaEffect()
{
}

void USearchAreaEffect::Init(AActor* NewOwner, UAbility* NewOwnerAbility /*= nullptr*/, EEffectTarget NewTarget /*= EEffectTarget::EE_Owner*/)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USearchAreaEffect::Init"));

	Super::Init(NewOwner, NewOwnerAbility, NewTarget);

	//if you want the system to support one search area have it self as sub effects, this need to be in the Start() to avoid infinite loop
	PrepareSubEffects();
}

void USearchAreaEffect::RestartEffect_Implementation(bool Relaunch /*= false*/)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USearchAreaEffect::RestartEffect"));

	Super::RestartEffect_Implementation(Relaunch);

	PointTargets.Empty();

	const int NumEffects = SubEffects.Num();
	for (int i = 0; i < NumEffects; i++)
		SubEffects[i]->RestartEffect(false);//this effects always have to restart completely
}

void USearchAreaEffect::PrepareSubEffects()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USearchAreaEffect::PrepareSubEffects"));

	UEffect* NewEffect = nullptr;

	const int NumEffects = SubEffectsBP.Num();
	for (int i = 0; i < NumEffects; i++)
	{
		const EEffectType CurrentType = Cast<UEffect>(SubEffectsBP[i]->GetDefaultObject())->GetEffectType();

		if (CurrentType == EEffectType::EE_SpawnActor)
			NewEffect = NewObject<USpawnActorEffect>(this, SubEffectsBP[i]);
		else if (CurrentType == EEffectType::EE_SearchArea)
			NewEffect = NewObject<USearchAreaEffect>(this, SubEffectsBP[i]);
		else if (CurrentType == EEffectType::EE_ModifyStat)
			NewEffect = NewObject<UModifyStatEffect>(this, SubEffectsBP[i]);

		NewEffect->Init(OwnerActor, OwnerAbility);
		SubEffects.Add(NewEffect);
	}
}

void USearchAreaEffect::StartEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USearchAreaEffect::StartEffect"));

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActor);
	Params.ClearIgnoredComponents();

	TArray<FOverlapResult> AllOverlaps;
	FCollisionQueryParams SphereParams;
	FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);

	FVector LaunchLocation = (TargetType == EEffectTarget::EE_Owner ? OwnerActor->GetActorLocation() : FVector::ZeroVector);

	bool Hit = OwnerActor->GetWorld()->OverlapMultiByObjectType(AllOverlaps, LaunchLocation, FQuat(FRotator(0)), HMR_TARGET_CHANNEL, Shape, SphereParams);
	DrawDebugSphere(OwnerActor->GetWorld(), LaunchLocation, Radius, 20, FColor::Red, false, 5.0f, 0, 1.0f);

	const int NumOverlaped = AllOverlaps.Num();
	for (int i = 0; i < NumOverlaped; i++)
		ActorTargets.AddUnique(AllOverlaps[i].GetActor());
	
	CurrentState = EEffectState::EE_Updating;
}

void USearchAreaEffect::UpdateEffect_Implementation(float ElapsedTime)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USearchAreaEffect::UpdateEffect"));

	const int NumElements = (SearchTarget == ESearchTarget::ES_Actors ? ActorTargets.Num() : PointTargets.Num());
	for (auto Element : ActorTargets)
	{
		TScriptInterface<IStatsActor> StatsActor = Element;
		if (StatsActor.GetObject())
		{
			const int NumSubEffects = SubEffects.Num();
			for (int i = 0; i < NumSubEffects; i++)
			{
				SubEffects[i]->SetTargetType(EEffectTarget::EE_Target);
				SubEffects[i]->SetTargetActor(Element);
				SubEffects[i]->Update(ElapsedTime);
			}
		}

		for (auto SubEffect : SubEffects)
			SubEffect->RestartEffect();
	}

	CurrentState = EEffectState::EE_Ending;
}

void USearchAreaEffect::EndEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USearchAreaEffect::EndEffect"));

	CurrentState = EEffectState::EE_Finished;
}
