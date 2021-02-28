// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitiesModule/Interfaces/AbilityActor.h"
#include "AbilityComponent.generated.h"

class UAbility;
class UEffect;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABILITIESMODULE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();
	virtual ~UAbilityComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Return all the C++ ability object that the owner has
	UFUNCTION(BlueprintCallable, Category = "HMR Ability Component", meta = (DisplayName = "Get Abilities"))
	TArray<UAbility*>& GetAbilities();

	//Set all the C++ ability objects
	UFUNCTION(BlueprintCallable, Category = "HMR Ability Component", meta = (DisplayName = "Set Abilities"))
	void SetAbilities(const TArray<UAbility*>& NewAbilities);

	//Set an ability blueprint to a valid C++ object
	UFUNCTION(BlueprintCallable, Category = "HMR Ability Component", meta = (DisplayName = "Prepare Ability"))
	UAbility* PrepareAbility(const TSubclassOf<UAbility>& NewAbility, AActor* Owner);

	//Add a C++ ability object to the component
	UFUNCTION(BlueprintCallable, Category = "HMR Ability Component", meta = (DisplayName = "Add Ability"))
	void AddAbility(UAbility* NewAbility);

	//Launch the ability if it can be launched
	UFUNCTION(BlueprintCallable, Category = "HMR Ability Component", meta = (DisplayName = "Execute Ability"))
	void ExecuteAbility(UAbility* Ability);

	//Launch the effect with the owner as target
	UFUNCTION(BlueprintCallable, Category = "HMR Ability Component", meta = (DisplayName = "Execute Effect"))
	void ExecuteEffect(UEffect* Effect);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//C++ abilities object
	UPROPERTY(BlueprintReadWrite, Category = "HMR Ability|Ability Component", meta = (DisplayName = "Abilities", AllowPrivateAccess = "true"))
	TArray<UAbility*> Abilities;

	//C++ effects object
	UPROPERTY(BlueprintReadWrite, Category = "HMR Ability|Ability Component", meta = (DisplayName = "Effects", AllowPrivateAccess = "true"))
	TArray<UEffect*> Effects;
};
