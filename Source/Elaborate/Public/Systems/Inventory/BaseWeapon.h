// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Inventory/BaseItem.h"
#include "Systems/Inventory/CommonTypes.h"

#include "BaseWeapon.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class ELABORATE_API UBaseWeapon : public UBaseItem
{
	GENERATED_BODY()
	
public:
	/** Base Damage the weapon deals */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon | Stats")
	float MinBaseDamage = 0.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon | Stats")
	float MaxBaseDamage = 10.f;

	/** Attack per second */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon | Stats")
	float AttackRate = 0.0f;

	/** How often the weapon does a critical hit */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon | Stats")
	float CritRate = 0.0f;

	/** In range 1.0... where 1.0 is an ordinary DMG */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon | Stats")
	float CritDamage = 1.0f;

	/** Current level of the weapon */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon | Upgrade")
	int32 UpgradeLevel = 0;

	/** Maximum level of the weapon */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Weapon | Upgrade")
	int32 MaxUpgradeLevel = 0;
};
