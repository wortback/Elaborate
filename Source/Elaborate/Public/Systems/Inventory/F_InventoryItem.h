// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Inventory/BaseItem.h"
#include "Systems/Inventory/CommonTypes.h"
#include "F_InventoryItem.generated.h"


class UInventoryComponent;


constexpr int32 DEFAULT_INDEX_LOCATION = -1;

/**
 * @struct F_InventoryItem
 * Item struct used for storing inventory related item information.
 * 
 */
USTRUCT(Blueprintable)
struct F_InventoryItem
{
    GENERATED_BODY()

public:
    /** Pointer to item's class */
    UPROPERTY(EditAnywhere)
    TSubclassOf<UBaseItem> ItemClass;

    /** Quantity of item */
    UPROPERTY(EditAnywhere)
    int32 Quantity;

    /** Inventory which the item belongs to */
    UInventoryComponent* OwningInventory;

    /** Location of the item in the inventory */
    int32 IndexLocation;

    /** Type of item */
    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    // Default constructor and Destructor
    F_InventoryItem();
    ~F_InventoryItem();

    // Other constructors
    F_InventoryItem(const F_InventoryItem& Item);

    /** Reset the item to it's defaults (only its class, quantity, and type). Preserves the index */
    void ClearItem();
    
};