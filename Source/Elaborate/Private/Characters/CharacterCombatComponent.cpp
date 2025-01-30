// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Elaborate.h"

UCharacterCombatComponent::UCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	
}


void UCharacterCombatComponent::PerformMeleeAttack(bool bIsHeavy)
{
	if (bIsHeavy)
	{
		if (Stamina - HeavyAttackStaminaConsuption < 0)
		{
			UE_LOG(LogCombatSystem, Warning, TEXT("Can't perform a heavy attack since the stamina is low."));
			return;
		}
		// TODO
	}
	// TODO
}

void UCharacterCombatComponent::PerformRangedAttack()
{
	// TODO
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

