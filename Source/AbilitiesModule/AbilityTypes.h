// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "UObject/ObjectMacros.h"
#include "AbilityTypes.generated.h"

UENUM(BlueprintType, DisplayName = "Ability Input Mode")
enum class EAbilityInputMode : uint8
{
	EI_None							UMETA(DisplayName = "None", Hidden),
	EI_OneHit						UMETA(DisplayName = "One Hit"),
	EI_ChargeAndRelease				UMETA(DisplayName = "Charge And Release")
};

UENUM(BlueprintType, DisplayName = "Effect Target")
enum class EEffectTarget : uint8
{
	EE_None							UMETA(DisplayName = "None", Hidden),
	EE_Owner						UMETA(DisplayName = "Owner"),
	EE_Target						UMETA(DisplayName = "Target")
};

UENUM(BlueprintType)
enum class EAnimationID : uint8
{
	EA_None							UMETA(DisplayName = "None", Hidden),
	EA_Beam							UMETA(DisplayName = "Beam"),
	EA_Projectile					UMETA(DisplayName = "Projectile"),
	EA_Conjure						UMETA(DisplayName = "Conjure"),
	EA_Lift							UMETA(DisplayName = "Lift"),
	EA_PalmBlast					UMETA(DisplayName = "Palm Blast")
};

//Used in the abilities effect array
USTRUCT(BlueprintType, DisplayName = "Effect Data")
struct ABILITIESMODULE_API FEffectData
{
	GENERATED_BODY();

public:
	FEffectData()
	{
		Effect = NULL;
		Target = EEffectTarget::EE_Owner;
	}

	FEffectData(TSubclassOf<class UEffect> NewEffect, EEffectTarget NewTarget)
		: Effect(NewEffect),
		Target(NewTarget)
	{}

	//Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Effect"))
	TSubclassOf<class UEffect> Effect;

	//Target for the effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Ability", meta = (DisplayName = "Target"))
	EEffectTarget Target;
};