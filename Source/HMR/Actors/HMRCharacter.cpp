// Copyright Epic Games, Inc. All Rights Reserved.

#include "HMRCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Particles/ParticleSystemComponent.h"
#include "StatsModule/Components/StatsComponent.h"
#include "AbilitiesModule/Components/AbilityComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkinnedMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Logging/LogMacros.h"
#include "HMR/HMR.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AHMRCharacter::AHMRCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->SetupAttachment(FirstPersonCameraComponent);
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -165.f));

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	AbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));

	LeftHandSelectedAbility = nullptr;
	RightHandSelectedAbility = nullptr;
	LeftHandIndex = 0;
	RightHandIndex = 0;
	LMBPressed = false;
	RMBPressed = false;
}

AHMRCharacter::~AHMRCharacter()
{
}

void AHMRCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	PrepareAbilities();
	IStatsActor::Execute_RefreshOwnerVars(this, ENumStats::ES_None);
}

void AHMRCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	IStatsActor::Execute_CheckLife(this);
}

// Input
void AHMRCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind left hand Ability events
	PlayerInputComponent->BindAction("LeftHand", IE_Pressed, this, &AHMRCharacter::LeftHand);
	PlayerInputComponent->BindAction("LeftHand", IE_Released, this, &AHMRCharacter::LeftHandReleased);
	PlayerInputComponent->BindAction("NextLeftHandAbility", IE_Pressed, this, &AHMRCharacter::NextLeftHandAbility);
	PlayerInputComponent->BindAction("NextLeftHandAbility", IE_Released, this, &AHMRCharacter::NextLeftHandAbilityReleased);

	// Bind Right hand Ability events
	PlayerInputComponent->BindAction("RightHand", IE_Pressed, this, &AHMRCharacter::RightHand);
	PlayerInputComponent->BindAction("RightHand", IE_Released, this, &AHMRCharacter::RightHandReleased);
	PlayerInputComponent->BindAction("NextRightHandAbility", IE_Pressed, this, &AHMRCharacter::NextRightHandAbility);
	PlayerInputComponent->BindAction("NextRightHandAbility", IE_Released, this, &AHMRCharacter::NextRightHandAbilityReleased);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AHMRCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHMRCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHMRCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHMRCharacter::LookUpAtRate);
}

void AHMRCharacter::ChangeAbility(UAbility*& Ability, TArray<UAbility*>& Abilities, int& CurrentIndex)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::ChangeAbility"));

	const int PreviousIndex = CurrentIndex;

	//check if can change ability
	if (Abilities.Num() == 0
		|| (Ability && (Ability->IsRunning() || Ability->GetInputPressed())))
		return;

	//Get valid index
	if (CurrentIndex < Abilities.Num() - 1)
		CurrentIndex++;
	else
		CurrentIndex = 0;

	Ability = Abilities[CurrentIndex];

	//BP call
	AbilityChange(PreviousIndex != CurrentIndex);
}

UAbilityComponent* AHMRCharacter::GetAbilityComponent_Implementation() const
{
	return AbilityComp;
}

UAbility* AHMRCharacter::PrepareAbility_Implementation(TSubclassOf<UAbility> AbilityBP, AActor* AbilityOwner)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::PrepareAbility"));

	return AbilityComp->PrepareAbility(AbilityBP, AbilityOwner);
}

void AHMRCharacter::PrepareAbilities()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::PrepareAbilities"));

	//Left hand abilities
	int LeftHandAbilitiesNum = LeftHandAbilitiesBP.Num();
	for (int i = 0; i < LeftHandAbilitiesNum; i++)
	{
		UAbility* NewAbility = IAbilityActor::Execute_PrepareAbility(this, LeftHandAbilitiesBP[i], this);
		
		IAbilityActor::Execute_AddAbility(this, NewAbility);
		LeftHandAbilities.Add(NewAbility);
	}

	if(LeftHandAbilitiesNum)
		LeftHandSelectedAbility = LeftHandAbilities[0];

	//Right hand abilities
	const int RightHandAbilitiesNum = RightHandAbilitiesBP.Num();
	for (int i = 0; i < RightHandAbilitiesNum; i++)
	{
		UAbility* NewAbility = IAbilityActor::Execute_PrepareAbility(this, RightHandAbilitiesBP[i], this);
		IAbilityActor::Execute_AddAbility(this, NewAbility);
		RightHandAbilities.Add(NewAbility);
	}

	if (RightHandAbilitiesNum)
		RightHandSelectedAbility = RightHandAbilities[0];
}

void AHMRCharacter::AddAbility_Implementation(UAbility* Ability)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::AddAbility"));

	AbilityComp->AddAbility(Ability);
}

void AHMRCharacter::LaunchAbility_Implementation(UAbility* Ability)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::LaunchAbility"));

	if (Ability)
	{
		AbilityComp->ExecuteAbility(Ability);
		Ability->SetInputPressed(true);
	}
}

USkeletalMesh* AHMRCharacter::GetSkeletalMesh_Implementation() const
{
	return GetMesh()->SkeletalMesh;
}

USkeletalMeshComponent* AHMRCharacter::GetSkeletalMeshComponent_Implementation() const
{
	return GetMesh();
}

UCameraComponent* AHMRCharacter::GetCameraComponent_Implementation() const
{
	return FirstPersonCameraComponent;
}

FRotator AHMRCharacter::GetOwnerRotation_Implementation() const
{
	return FirstPersonCameraComponent->GetComponentRotation();
}

UStatsComponent* AHMRCharacter::GetStatsComponent_Implementation() const
{
	return StatsComp;
}

void AHMRCharacter::Heal_Implementation(float Value, bool Percent /*= false*/)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::Heal"));

	const float PreviousValue = StatsComp->GetLifeValue();

	StatsComp->ModifyNumericStat(ENumStats::ES_Life, FNumericStatMod(FMath::Abs(Value), Percent));

	//if has been change, BP call
	if(PreviousValue != StatsComp->GetLifeValue())
		CurrentLifeChange(true);
}

void AHMRCharacter::ReceiveDamage_Implementation(float Value, bool Percent /*= false*/)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::ReceiveDamage"));

	const float PreviousValue = StatsComp->GetLifeValue();

	StatsComp->ModifyNumericStat(ENumStats::ES_Life, FNumericStatMod(-FMath::Abs(Value), Percent));

	//if has been change, BP call
	if (PreviousValue != StatsComp->GetLifeValue())
		CurrentLifeChange(false);
}

void AHMRCharacter::RecoverMana_Implementation(float Value, bool Percent /*= false*/)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::RecoverMana"));

	const float PreviousValue = StatsComp->GetManaValue();

	StatsComp->ModifyNumericStat(ENumStats::ES_Mana, FNumericStatMod(FMath::Abs(Value), Percent));

	//if has been change, BP call
	if (PreviousValue != StatsComp->GetManaValue())
		CurrentManaChange(true);
}

void AHMRCharacter::ConsumeMana_Implementation(float Value, bool Percent /*= false*/)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::ConsumeMana"));

	const float PreviousValue = StatsComp->GetLifeValue();

	StatsComp->ModifyNumericStat(ENumStats::ES_Mana, FNumericStatMod(-FMath::Abs(Value), Percent));

	//if has been change, BP call
	if (PreviousValue != StatsComp->GetManaValue())
		CurrentManaChange(false);
}

void AHMRCharacter::ModifyStat_Implementation(ENumStats Stat, float Value, bool Percent /*= false*/)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::ModifyStat"));

	const FNumericStatValues& CurrentStat = StatsComp->GetStat(Stat);
	const float PreviousValue = CurrentStat.Value;

	StatsComp->ModifyNumericStat(Stat, FNumericStatMod(Value, Percent));

	//if has been change, BP call
	if (PreviousValue != CurrentStat.Value)
		StatChange(Stat, CurrentStat.Value, PreviousValue, PreviousValue < CurrentStat.Value);
}

void AHMRCharacter::ModifyMoveSpeed_Implementation(float Value, bool Percent /*= false*/)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::ModifyMoveSpeed"));

	const FNumericStatValues& CurrentStat = StatsComp->GetStat(ENumStats::ES_MoveSpeed);
	const float PreviousValue = CurrentStat.Value;

	StatsComp->ModifyNumericStat(ENumStats::ES_MoveSpeed, FNumericStatMod(Value, Percent));
	
	//if has been change, BP call
	if (PreviousValue != CurrentStat.Value)
		MoveSpeedChange(PreviousValue < CurrentStat.Value);
}

void AHMRCharacter::RefreshOwnerVars_Implementation(ENumStats Stat /*= ENumStats::ES_None*/)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::RefreshOwnerVars"));
	
	//if move speed is change, update movement component walk speed
	if (Stat == ENumStats::ES_MoveSpeed || Stat == ENumStats::ES_None)
		GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetMoveSpeedValue();
}

void AHMRCharacter::Kill_Implementation()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::Kill"));

	IStatsActor::Execute_OnDeath(this);
}

void AHMRCharacter::OnDeath_Implementation()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::OnDeath"));

	TArray<UAbility*> CharacterAbilities = AbilityComp->GetAbilities();
	const int AbilitiesNum = CharacterAbilities.Num();
	
	for (int i = 0; i < AbilitiesNum; i++)
		CharacterAbilities[i]->CancelAbility();

	LeftHandSelectedAbility = nullptr;
	RightHandSelectedAbility = nullptr;
	LeftHandAbilities.Empty();
	RightHandAbilities.Empty();
	
	Destroy();
}

void AHMRCharacter::CheckLife_Implementation()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::CheckLife"));

	if (StatsComp->GetLifeValue() == 0)
		IStatsActor::Execute_OnDeath(this);
}

UCameraComponent* AHMRCharacter::GetFirstPersonCameraComponent() const
{
	return FirstPersonCameraComponent;
}

const TArray<TSubclassOf<UAbility>>& AHMRCharacter::GetLeftHandAbilitiesBP() const
{
	return LeftHandAbilitiesBP;
}

const TArray<UAbility*>& AHMRCharacter::GetLeftHandAbilities() const
{
	return LeftHandAbilities;
}

UAbility* AHMRCharacter::GetLeftHandSelectedAbility() const
{
	return LeftHandSelectedAbility;
}

const TArray<TSubclassOf<UAbility>>& AHMRCharacter::GetRightHandAbilitiesBP() const
{
	return RightHandAbilitiesBP;
}

const TArray<UAbility*>& AHMRCharacter::GetRightHandAbilities() const
{
	return RightHandAbilities;
}

UAbility* AHMRCharacter::GetRightHandSelectedAbility() const
{
	return RightHandSelectedAbility;
}

void AHMRCharacter::LeftHand()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::LeftHand"));

	IAbilityActor::Execute_LaunchAbility(this, LeftHandSelectedAbility);
	LMBPressed = true;
}

void AHMRCharacter::LeftHandReleased()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::LeftHandReleased"));

	if (LeftHandSelectedAbility)
		LeftHandSelectedAbility->SetInputPressed(false);
	
	LMBPressed = false;
}

void AHMRCharacter::NextLeftHandAbility()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::NextLeftHandAbility"));

	if (LeftHandAbilities.Num() == 0 
		|| (LeftHandSelectedAbility && LeftHandSelectedAbility->IsRunning()))
		return;

	ChangeAbility(LeftHandSelectedAbility, LeftHandAbilities, LeftHandIndex);
	if(LeftHandAbilities.Num())
		LeftHandAbilityChange.Broadcast(LeftHandAbilities, LeftHandIndex);
}

void AHMRCharacter::NextLeftHandAbilityReleased()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::NextLeftHandAbilityReleased"));
}

void AHMRCharacter::RightHand()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::RightHand"));

	IAbilityActor::Execute_LaunchAbility(this, RightHandSelectedAbility);
	RMBPressed = true;
}

void AHMRCharacter::RightHandReleased()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::RightHandReleased"));

	if (RightHandSelectedAbility)
		RightHandSelectedAbility->SetInputPressed(false);

	RMBPressed = false;
}

void AHMRCharacter::NextRightHandAbility()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::NextRightHandAbility"));

	if (RightHandAbilities.Num() == 0
		|| (RightHandSelectedAbility && RightHandSelectedAbility->IsRunning()))
		return;

	ChangeAbility(RightHandSelectedAbility, RightHandAbilities, RightHandIndex);

	if (RightHandAbilities.Num())
		RightHandAbilityChange.Broadcast(RightHandAbilities, RightHandIndex);
}

void AHMRCharacter::NextRightHandAbilityReleased()
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::NextRightHandAbilityReleased"));
}

void AHMRCharacter::MoveForward(float Value)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::MoveForward"));

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AHMRCharacter::MoveRight(float Value)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::MoveRight"));

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AHMRCharacter::TurnAtRate(float Rate)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::TurnAtRate"));

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHMRCharacter::LookUpAtRate(float Rate)
{
	UE_LOG(HMRModuleLog, Display, TEXT("AHMRCharacter::LookUpAtRate"));

	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}