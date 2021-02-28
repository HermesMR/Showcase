// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesModule/Effects/SpawnActorEffect.h"
#include <GameFramework/Actor.h>
#include "AbilitiesModule/AbilitiesModule.h"
#include "AbilitiesModule/Abilities/Ability.h"
#include "Engine/SkeletalMeshSocket.h"
#include <DrawDebugHelpers.h>
#include "GameFramework/Character.h"

USpawnActorEffect::USpawnActorEffect()
{
	EffectType = EEffectType::EE_SpawnActor;
	SpawnInSocket = false;
	AttachToOwner = false;
	LocationOffset = FVector::ZeroVector;
	SpawnLocation = FVector::ZeroVector;
	NewActor = nullptr;
}

USpawnActorEffect::~USpawnActorEffect()
{
}

void USpawnActorEffect::RestartEffect_Implementation(bool Relaunch /*= false*/)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USpawnActorEffect::RestartEffect"));

	Super::RestartEffect_Implementation(Relaunch);

	//need this for the abilities that can hold the input
	if (!Relaunch && NewActor)
	{
		NewActor->Destroy();
		NewActor = nullptr;
	}
}

void USpawnActorEffect::StartEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USpawnActorEffect::StartEffect"));

	//If targettype is "Target" will have to be set it before effect launch
	if (TargetType == EEffectTarget::EE_Owner)
		TargetActor = OwnerActor;

	CurrentState = EEffectState::EE_Updating;
}

void USpawnActorEffect::UpdateEffect_Implementation(float ElapsedTime)
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USpawnActorEffect::UpdateEffect"));

	//Actor target
	if (TargetActor)
	{
		if (!AttachToOwner
			|| (AttachToOwner && NewActor == nullptr))
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParam.Owner = OwnerActor;
			SpawnParam.Instigator = CastChecked<APawn>(OwnerActor);

			UWorld * World = GEngine->GetWorldFromContextObject(SpawnParam.Owner, EGetWorldErrorMode::LogAndReturnNull);

			FTransform ActorSpawnTransform;
			ActorSpawnTransform.SetLocation(FVector::ZeroVector);
			ActorSpawnTransform.SetRotation(FQuat(FRotator::ZeroRotator));

			if (SpawnInSocket)
			{
				const USkeletalMeshSocket* Socket = IAbilityActor::Execute_GetSkeletalMesh(SpawnParam.Owner)->FindSocket(SocketName);

				if (IsValid(Socket))
					SpawnLocation = Cast<ACharacter>(SpawnParam.Owner)->GetMesh()->GetSocketLocation(SocketName);
			}

			ActorSpawnTransform.SetLocation(SpawnLocation);
			ActorSpawnTransform.SetRotation(FQuat(IAbilityActor::Execute_GetOwnerRotation(SpawnParam.Owner)));

			NewActor = World->SpawnActorDeferred<AActor>(Actor, ActorSpawnTransform, SpawnParam.Owner, Cast<APawn>(SpawnParam.Owner), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			
			NewActor->FinishSpawning(ActorSpawnTransform);

			if (AttachToOwner)
				NewActor->AttachToComponent(Cast<USceneComponent>(IAbilityActor::Execute_GetCameraComponent(SpawnParam.Owner)), FAttachmentTransformRules::KeepWorldTransform, NAME_None);
		}

		CurrentState = EEffectState::EE_Ending;
	}
	else//Point target TODO
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParam.Owner = OwnerActor;
		SpawnParam.Instigator = CastChecked<APawn>(OwnerActor);

		UWorld * World = GEngine->GetWorldFromContextObject(SpawnParam.Owner, EGetWorldErrorMode::LogAndReturnNull);

		//Default location
		if (TargetPoint == FVector::ZeroVector)
			TargetPoint = SpawnParam.Owner->GetActorLocation() + SpawnParam.Owner->GetActorForwardVector() * 600.0f;

		FTransform ActorSpawnTransform;
		ActorSpawnTransform.SetLocation(TargetPoint);
		ActorSpawnTransform.SetRotation(FQuat(SpawnParam.Owner->GetActorRotation()));

		NewActor = World->SpawnActorDeferred<AActor>(Actor, ActorSpawnTransform, SpawnParam.Owner, Cast<APawn>(SpawnParam.Owner), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewActor->FinishSpawning(ActorSpawnTransform);

		CurrentState = EEffectState::EE_Ending;
	}
}

void USpawnActorEffect::EndEffect_Implementation()
{
	UE_LOG(AbilitiesModuleLog, Display, TEXT("USpawnActorEffect::EndEffect"));

	//need this for the abilities that can hold the input
	if (!InputPressed && AttachToOwner && NewActor)
	{
		NewActor->Destroy();
		NewActor = nullptr;
	}

	CurrentState = EEffectState::EE_Finished;
}
