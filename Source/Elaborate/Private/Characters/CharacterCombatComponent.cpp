// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

#include "Systems/Interfaces/CombatWeaponInterface.h"
#include "Systems/Interfaces/CombatCharacterInterface.h"
#include "Systems/Inventory/InventoryComponent.h"
#include "Systems/Inventory/WorldItem.h"
#include "Systems/Inventory/BaseWeapon.h"
#include "Systems/Inventory/WorldWeapons/MeleeWeaponWorld.h"

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
		UE_LOG(LogCombatSystem, Log, TEXT("Trying to fetch the equipped weapon from the inventory component..."));

		if (TryFetchWeaponFromInventory())
		{
			UWorld* World = GetWorld();
			if (!World) return;

			// Set spawn location (e.g., character's position)
			FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(50, 0, 0);
			FRotator SpawnRotation = FRotator::ZeroRotator;

			UBaseWeapon* DefaultWeapon = Cast<UBaseWeapon>(EquippedWeaponClass->GetDefaultObject());
			if (DefaultWeapon)
			{
				switch (DefaultWeapon->WeaponType)
				{
				case EWeaponType::EWT_Melee:
				{
					EquippedWeapon = World->SpawnActorDeferred<AMeleeWeaponWorld>(AMeleeWeaponWorld::StaticClass(),
						FTransform(SpawnRotation, SpawnLocation));
					if (EquippedWeapon)
					{
						EquippedWeapon->SetItemClass(EquippedWeaponClass);  // Set before spawning
						EquippedWeapon->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
					}
				}
				default:
					break;
				}
			}
			EquipWeapon(EquippedWeapon);
		}
		else
		{
			UE_LOG(LogCombatSystem, Error, TEXT("Failed to attack since no weapon is equipped."));
			return;
		}

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
	else
	{
		// Perform Normal Attack
		CombatWeapon->Attack();
	}

	UAnimMontage* Montage = bIsHeavy ? HeavyAttackMontage : AttackMontage;

	if (Montage)
	{
		PlayAttackMontage(Montage);
	}
	else
	{
		UE_LOG(LogCombatSystem, Warning, TEXT("No attack montage set for this type of attack."));
	}
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

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		UE_LOG(LogCombatSystem, Log, TEXT("Owner Character of the combat component is successfully initialised."));
	}
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

void UCharacterCombatComponent::PlayAttackMontage(UAnimMontage* Montage)
{
	if (!AttackMontage)
	{
		UE_LOG(LogCombatSystem, Warning, TEXT("No attack montage provided."));
		return;
	}

	if (OwnerCharacter && OwnerCharacter->GetMesh())
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
	}
}

void UCharacterCombatComponent::EquipWeapon(AWorldItem* Weapon)
{
	if (OwnerCharacter && OwnerCharacter->GetMesh())
	{
		Weapon->AttachToComponent(OwnerCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			TEXT("WeaponSocket"));
	}

}

bool UCharacterCombatComponent::TryFetchWeaponFromInventory()
{
	if (OwnerCharacter)
	{
		UInventoryComponent* InventoryComp = OwnerCharacter->FindComponentByClass<UInventoryComponent>();
		if (InventoryComp)
		{
			UE_LOG(LogCombatSystem, Log, TEXT("Inventory Component Found!"));

			EquippedWeaponClass = InventoryComp->GetEquippedWeaponClass();
			if (EquippedWeaponClass)
			{
				return true;
			}
			return false;
		}
		else
		{
			UE_LOG(LogCombatSystem, Warning, TEXT("Cannot find the inventory component of the owner character."));
		}
	}
	return false;
}
