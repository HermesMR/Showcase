// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Camera/CameraComponent.h"
#include "AbilityActor.generated.h"

class USkeletalMesh;
class UAbilityComponent;
class UCameraComponent;
class UAbility;
class UEffect;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UAbilityActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ABILITIESMODULE_API IAbilityActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*!
	 * @fn	virtual UAbilityComponent* IAbilityActor::GetAbilityComponent() = 0;
	 *
	 * @brief	Gets ability component
	 * @returns	Null if it fails, else the ability component.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IAbilityActor::Get Ability Component"))
	UAbilityComponent* GetAbilityComponent();
	virtual UAbilityComponent* GetAbilityComponent_Implementation() const = 0;

	/**
	 * @fn	virtual UAbility* IAbilityActor::PrepareAbility(const TSubclassOf<UAbility>& AbilityBP) = 0;
	 *
	 * @brief	Creates the C++ object of the ability BluePrint.
	 *
	 * @param 	Ability BluePrint to create the C++ obj.
	 *
	 * @returns	Null if it fails, else a pointer to the UAbility.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IAbilityActor::Get Ability Component"))
	UAbility* PrepareAbility(UPARAM(ref) TSubclassOf<UAbility>& AbilityBP, AActor* AbilityOwner);
	virtual UAbility* PrepareAbility_Implementation(TSubclassOf<UAbility> AbilityBP, AActor* AbilityOwner) = 0;
	

	/**
	 * @fn	virtual void IAbilityActor::AddAbility(UAbility* Ability) = 0;
	 *
	 * @brief	Adds an ability
	 *
	 * @param Ability obj to add to the component
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IAbilityActor::AddAbility"))
	void AddAbility(UAbility* Ability);
	virtual void AddAbility_Implementation(UAbility* Ability) = 0;

	/**
	 * @fn	virtual void IAbilityActor::ExecuteAbility(UAbility* Ability) = 0;
	 *
	 * @brief	Launch the ability send by parameter
	 *
	 * @param Ability obj to launch
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IAbilityActor::Launch Ability"))
	void LaunchAbility(UAbility* Ability);
	virtual void LaunchAbility_Implementation(UAbility* Ability) = 0;

	/*!
	 * @fn	virtual USkeletalMesh* IAbilityActor::GetSkeletalMesh() = 0;
	 *
	 * @brief	Gets skeletal mesh
	 * @returns	Null if it fails, else the skeletal mesh.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IAbilityActor::Get Skeletal Mesh"))
	USkeletalMesh* GetSkeletalMesh() const;
	virtual USkeletalMesh* GetSkeletalMesh_Implementation() const = 0;

	/**
	 * @fn	virtual USkeletalMesh* IAbilityActor::GetSkeletalMeshComponent() const = 0;
	 *
	 * @brief	Gets skeletal mesh component
	 *
	 * @returns	Null if it fails, else the skeletal mesh component.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IAbilityActor::Get Skeletal Mesh Component"))
	USkeletalMeshComponent* GetSkeletalMeshComponent() const;
	virtual USkeletalMeshComponent* GetSkeletalMeshComponent_Implementation() const = 0;

	/**
	 * @fn	virtual UCameraComponent* IAbilityActor::GetCameraComponent() const = 0;
	 *
	 * @brief	Gets camera component
	 *
	 * @returns	Null if it fails, else the camera component.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IAbilityActor::Get Camera Component"))
	UCameraComponent* GetCameraComponent() const;
	virtual UCameraComponent* GetCameraComponent_Implementation() const = 0;

	/*!
	 * @fn	virtual FRotator IAbilityActor::GetOwnerRotation() = 0;
	 *
	 * @brief	Gets the owner rotation
	 * @returns	The owner rotation.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "IAbilityActor::Get Owner Rotation"))
	FRotator GetOwnerRotation() const;
	virtual FRotator GetOwnerRotation_Implementation() const = 0;

protected:
	/*! @brief	The ability component */
	UAbilityComponent* AbilityComp;
};
