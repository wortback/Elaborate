// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Inventory/BaseItem.h"
#include "Systems/Inventory/CommonTypes.h"

#include "BaseWeapon.generated.h"

UENUM()
enum class EWeaponType : uint8
{
	EWT_Melee,
	EWT_Ranged
};
/**
 * 
 */
UCLASS(Blueprintable)
class ELABORATE_API UBaseWeapon : public UBaseItem
{
	GENERATED_BODY()
	
public:
	/** Base Damage the weapon deals */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Stats")
	float MinBaseDamage = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Stats")
	float MaxBaseDamage = 10.f;

	/** Attack per second */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Stats")
	float AttackRate = 0.0f;

	/** How often the weapon does a critical hit */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Stats")
	float CritRate = 0.0f;

	/** In range 1.0... where 1.0 is an ordinary DMG */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Stats")
	float CritDamage = 1.0f;

	/** Current level of the weapon */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Upgrade")
	int32 UpgradeLevel = 0;

	/** Maximum level of the weapon */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Upgrade")
	int32 MaxUpgradeLevel = 0;

	/** Is melee or ranged? */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Type")
	EWeaponType WeaponType;


};
