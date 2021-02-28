// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UTILSMODULE_API UUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static FString GetEnumString(const FString& EnumName, uint8 EnumValue);
	//const FString& enumName, const T value, const FString& defaultValue) const
};
