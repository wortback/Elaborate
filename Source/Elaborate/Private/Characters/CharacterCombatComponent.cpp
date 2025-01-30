// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Systems/Interfaces/CombatWeaponInterface.h"
#include "Systems/Inventory/WorldItem.h"
#include "Systems/Inventory/BaseWeapon.h"

#include "Elaborate.h"

UCharacterCombatComponent::UCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UCharacterCombatComponent::PerformAttack(bool bIsHeavy)
{
	if (!EquippedWeapon)
	{
		UE_LOG(LogCombatSystem, Warning, TEXT("No weapon equipped!"));
		return;
	}

	ICombatWeaponInterface* CombatWeapon = Cast<ICombatWeaponInterface>(EquippedWeapon);
	if (!CombatWeapon)
	{
		UE_LOG(LogCombatSystem, Warning, TEXT("Equipped weapon doesn't implement CombatWeaponInterface!"));
		return;
	}

	if (bIsHeavy)
	{
		if (Stamina < HeavyAttackStaminaConsumption)
		{
			UE_LOG(LogCombatSystem, Warning, TEXT("Not enough stamina for a heavy attack!"));
			return; // Exit early instead of doing a normal attack
		}

		bIsAttackHeavy = true;
		CombatWeapon->HeavyAttack();
		return;
	}

	// Perform Normal Attack
	CombatWeapon->Attack();
}

void UCharacterCombatComponent::TakeDamage(float IncomingDamage)
{
	Health -= IncomingDamage * DefenceMultiplier;

	if (Health <= 0)
	{
		Die();
	}
}

void UCharacterCombatComponent::ConsumeStamina(float StaminaPoints)
{
	Stamina -= StaminaPoints;
	if (Stamina < 0)
	{
		Stamina = 0;
	}
}

void UCharacterCombatComponent::RegenerateHealth(float HealthPoints)
{
	Health += HealthPoints;
	if (Health)
	{
		Health = MaxHealth;
	}
}

void UCharacterCombatComponent::RegenerateStamina(float StaminaPoints)
{
	Stamina += StaminaPoints;
	if (Stamina > MaxStamina)
	{
		Stamina = MaxStamina;
	}
}

void UCharacterCombatComponent::Die()
{
	// TODO
	UE_LOG(LogCombatSystem, Warning, TEXT("Combat component owner died."));
}

float UCharacterCombatComponent::CalculateTotalAttackDMG(bool bIsMelee)
{
	UBaseWeapon* WeaponStats = Cast<UBaseWeapon>(EquippedWeapon->GetItemClass()->GetDefaultObject());
	if (!WeaponStats)
	{
		UE_LOG(LogCombatSystem, Warning, TEXT("Weapon Cast failed."));
		return 0.0f;
	}

	// Calc base DMG
	float TotalDMG = FMath::FRandRange(WeaponStats->MinBaseDamage, WeaponStats->MaxBaseDamage);
	TotalDMG += bIsMelee ? BaseMeleeDMG : BaseRangedDMG;
	
	// Apply crit multiplier if crit hit
	const float TotalCritRate = WeaponStats->CritRate + CritRate;

	float CritProb = FMath::FRand();
	if (CritProb < TotalCritRate)
	{
		UE_LOG(LogCombatSystem, Warning, TEXT("Landed a crit hit."));

		TotalDMG *= (WeaponStats->CritDamage + CritDMG) - 1.f;
	}

	// Apply extra if heavy
	if (bIsAttackHeavy)
	{
		TotalDMG *= HeavyAttackDMGMultiplier;
	}

	// Reset state after the damage calculation
	bIsAttackHeavy = false;
	UE_LOG(LogCombatSystem, Log, TEXT("Total damage equals %f"), TotalDMG);
	return TotalDMG;
}

void UCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}



bool UCharacterCombatComponent::CanAttack()
{
	if (bIsAttacking || bIsAttackOnCoolDown)
	{
		return false;
	}

	if (MovementComponent)
	{
		if (MovementComponent->IsFalling())
		{
			UE_LOG(LogCombatSystem, Warning, TEXT("Can't attack. The character is falling."));
			return false;
		}
	}

	return true;
}

void UCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

