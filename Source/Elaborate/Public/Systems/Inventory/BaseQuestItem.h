// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Inventory/BaseItem.h"
#include "BaseQuestItem.generated.h"


class AActor;
/**
 * 
 */
UCLASS()
class ELABORATE_API UBaseQuestItem : public UBaseItem
{
	GENERATED_BODY()

public:
	/** Name of the NPC who gave the player the item */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Quest")
	FText QuestGiver = FText::GetEmpty();

	/** Name of the quest that this item is related to */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Quest")
	FText QuestName = FText::GetEmpty();
};
