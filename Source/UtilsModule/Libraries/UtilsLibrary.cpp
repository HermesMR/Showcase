// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilsModule/Libraries/UtilsLibrary.h"
#include "UtilsModule/UtilsModule.h"
#include "UtilsModule/UtilsTypes.h"

FString UUtilsLibrary::GetEnumString(const FString& EnumName, uint8 EnumValue)
{
	UE_LOG(UtilsModuleLog, Display, TEXT("UUtilsLibrary::GetEnumString"));

	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	FString Return = "Error";

	if (pEnum)
	{
		Return = pEnum->GetNameStringByIndex(EnumValue);
	}
	
	return Return;
}