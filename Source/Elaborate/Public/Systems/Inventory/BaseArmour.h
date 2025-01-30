// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Inventory/BaseItem.h"
#include "Systems/Inventory/CommonTypes.h"

#include "BaseArmour.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class ELABORATE_API UBaseArmour : public UBaseItem
{
	GENERATED_BODY()
	
public:
	/** What minimum percentage of inflicted damage is ignored */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Armour")
	float MinDamageReduction = 0.1f;

	/** What maximum percentage of inflicted damage is ignored */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Armour")
	float MaxDamageReduction = 0.10f;

	/** Type of armour's passive effect */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Armour")
	EPassiveArmour ArmourPassive = EPassiveArmour::EPA_None;

	/** Scaler value for the passive effect */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Armour")
	float PassiveMultiplier = 0.0f;
};
