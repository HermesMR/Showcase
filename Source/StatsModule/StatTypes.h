// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "StatTypes.generated.h"

#define CONDITION_CURRENT_STATS(Stat) Stat == ENumStats::ES_Life || Stat == ENumStats::ES_Mana
#define CONDITION_MAX_STATS(Stat) Stat == ENumStats::ES_MaxLife || Stat == ENumStats::ES_MaxMana


#define STAT_VAR(Type, Name, MyCategory) \
				UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory, meta = (AllowPrivateAccess = "true")) \
				Type Name; \

#define STAT_GET_STRUCT(Name) \
				FORCEINLINE FNumericStatValues* Get##Name##Struct() \
				{ \
					return &Name; \
				}

#define STAT_GET(Type, Name) \
				FORCEINLINE Type Get##Name##Value() const \
				{ \
					return Name.Value; \
				}

#define STAT_SET(Type, Name) \
				FORCEINLINE void Set##Name##Value(Type NewValue) \
				{ \
					Name.Value = NewValue; \
				}

#define STAT_MODIFY(Type, Name) \
				FORCEINLINE void Modify##Name##Value(Type Value) \
				{ \
					Name.Value += Value; \
				}


//numeric stat
UENUM(BlueprintType)
enum class ENumStats : uint8
{
	ES_None							UMETA(DisplayName = "None", Hidden),
	ES_Life							UMETA(DisplayName = "Life"),
	ES_MaxLife						UMETA(DisplayName = "Max Life"),
	ES_LifeRegenTime				UMETA(DisplayName = "Life Regen Time"),
	ES_LifeRegenSpeed				UMETA(DisplayName = "Life Regen Speed"),
	ES_Mana							UMETA(DisplayName = "Mana"),
	ES_MaxMana						UMETA(DisplayName = "Max Mana"),
	ES_ManaRegenTime				UMETA(DisplayName = "Mana Regen Time"),
	ES_ManaRegenSpeed				UMETA(DisplayName = "Mana Regen Speed"),
	ES_MoveSpeed					UMETA(DisplayName = "Move Speed"),
	ES_Last							UMETA(DisplayName = "Last", Hidden)
};

USTRUCT(BlueprintType, Blueprintable, DisplayName = "Numeric Stat")
struct STATSMODULE_API FNumericStatValues
{
	GENERATED_BODY();

public:
	FNumericStatValues() {
		Stat = ENumStats::ES_None; Value = 0; BaseValue = 0; Additives = 0.0f; 	Multiplicitives = 0.0f;	}
	FNumericStatValues(float Value, ENumStats StatId)
		: Stat(StatId),
		Value(Value),
		BaseValue(Value)
	{ Additives = 0.0f; Multiplicitives = 0.0f; }

	//Stat ID
	UPROPERTY(BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Stat"))
	ENumStats Stat;

	//Stat current value
	UPROPERTY(BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Current Value"))
	float Value;

	//Stat base value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Value", UIMin = 0.0f))
	float BaseValue;

	//list of the additive values to the stat
	UPROPERTY(BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Additives"))
	float Additives;

	//list of the multiplicative values to the stat
	UPROPERTY(BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Multiplicitives"))
	float Multiplicitives;
};

//Data to modify a numeric stat
USTRUCT(BlueprintType, DisplayName = "Numeric Stat")
struct STATSMODULE_API FNumericStatMod
{
	GENERATED_BODY();

	FORCEINLINE FNumericStatMod()
	{
		//StatClass = EStatClass::ES_Numeric;
		Value = 0.0f;
		Percent = false;
	}

	FORCEINLINE FNumericStatMod(float NewValue, bool NewPercent)
	{
		//StatClass = EStatClass::ES_Numeric;
		Value = NewValue;
		Percent = NewPercent;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Value"))
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HMR Stats", meta = (DisplayName = "Percent"))
	bool Percent;
};