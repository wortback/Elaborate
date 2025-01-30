// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Systems/Inventory/CommonTypes.h"

#include "BaseItem.generated.h"


/**
 * @class UBaseItem
 * Abstract base class that stores information an item.
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class ELABORATE_API UBaseItem : public UObject
{
	GENERATED_BODY()


public:
	/** Mesh that displayed in the world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	UStaticMesh* DisplayMesh = nullptr;

	/** Thumbnail that is displayed in the inventory slot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	UTexture2D* Thumbnail = nullptr;

	/* Text that contains the item's description that will be displayed in the game upon adding the 
	item to the player's inventory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	FText ItemDescription = FText::FromString("Base Item Description");

	/** Name of the item that is displayed upon tracing it and in the inventory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	FText ItemName = FText::FromString("Base Item Name");

	/** True, if the item is stackable (multiple copies stored in oe inventory slot) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	bool bIsStackable = true;

	/** Max amount of the item a player can possess */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int32 StackSize = 10; 

	/** True if the item should despawn in the world */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	bool bShouldDie = false;

	/** For how long the item persists in the world before despawning */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float DespawnTime = 0.0f;

	/** What item group should this item be assigned to within the inventory component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	EItemType ItemType = EItemType::EIT_None;

	/** Price of the item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int32 Price = 0;

	/** Weight of the item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float Weight = 0;

	/** For how long the item persists in the world before despawning */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	EItemRarity ItemRarity = EItemRarity::EIR_Common;
};
