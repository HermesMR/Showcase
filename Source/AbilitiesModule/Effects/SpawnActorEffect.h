// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitiesModule/Effects/Effect.h"
#include "SpawnActorEffect.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DisplayName = "Spawn Actor Effect")
class ABILITIESMODULE_API USpawnActorEffect : public UEffect
{
	GENERATED_BODY()
	
public:
	USpawnActorEffect();
	~USpawnActorEffect();

	virtual void RestartEffect_Implementation(bool Relaunch = false) override;

protected:
	virtual void StartEffect_Implementation() override;
	virtual void UpdateEffect_Implementation(float ElapsedTime) override;
	virtual void EndEffect_Implementation() override;

private:
	//Actor to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Spawn Actor", meta = (DisplayName = "ActorToSpawn", UIMin = 0.0f, AllowPrivateAccess = "true"))
	TSubclassOf<AActor> Actor;

	//Spawn in socket, if true the actor will spawn in the specified socket, if not founded spawns in owner location and if it doesn't have owner will spawn in (0, 0, 0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Spawn Actor", meta = (DisplayName = "Spawn In Socket", AllowPrivateAccess = "true"))
	bool SpawnInSocket;

	//If true, will only spawn ONE actor, this actor will be attached to the owner (ex channeling attacks)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Spawn Actor", meta = (DisplayName = "Attach To Owner", AllowPrivateAccess = "true"))
	bool AttachToOwner;

	//Socket name to spawn actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Spawn Actor", meta = (DisplayName = "Socket Name", EditCondition = "SpawnInSocket", AllowPrivateAccess = "true"))
	FName SocketName;

	//Location Offset from the point os spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR|Effect|Spawn Actor", meta = (DisplayName = "Location Offset", AllowPrivateAccess = "true"))
	FVector LocationOffset;

	//Spawn Location
	UPROPERTY(BlueprintReadWrite, Category = "HMR|Effect|Spawn Actor", meta = (DisplayName = "Spawn Location", AllowPrivateAccess = "true"))
	FVector SpawnLocation;

	//Actor spawned
	UPROPERTY()
	AActor* NewActor;
};
