// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define STRINGIFY(Value) #Value

#define VAR_GET(Type, Name) \
				FORCEINLINE Type Get##Name() const \
				{ \
					return Name; \
				}

#define VAR_SET(Type, Name) \
				FORCEINLINE void Set##Name(Type Value) \
				{ \
					Name = Value; \
				}

#define VAR_MODIFY(Type, Name) \
				FORCEINLINE void Modify##Name(Type Value) \
				{ \
					Name += Value; \
				}

#define VAR_GET_SET(Type, Name) \
				VAR_GET(Type, Name) \
				VAR_SET(Type, Name)