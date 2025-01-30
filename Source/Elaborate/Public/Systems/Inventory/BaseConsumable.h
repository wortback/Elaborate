// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Inventory/BaseItem.h"
#include "Systems/Inventory/CommonTypes.h"

#include "BaseConsumable.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ELABORATE_API UBaseConsumable : public UBaseItem
{
	GENERATED_BODY()
	

public:
	/** What player's attribute does the consumable affect. Default is Health. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Stats")
	EAttributeType AttributeType = EAttributeType::EAT_Health;

	/** How the affected attribute is changed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Stats")
	EConsumableEffect EffectType = EConsumableEffect::ECE_InstantRegeneration;

	/** Affected attribute value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Stats")
	float AttributeValue = 1.0f;
};
