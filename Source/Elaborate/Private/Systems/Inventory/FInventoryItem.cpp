// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/FInventoryItem.h"
#include "Systems/Inventory/CommonTypes.h"
#include "Systems/Inventory/Defines.h"





FInventoryItem::FInventoryItem()
{
    ItemClass = UBaseItem::StaticClass();
    Quantity = 0;
    IndexLocation = DEFAULT_INDEX_LOCATION;
    OwningInventory = nullptr;
    ItemType = EItemType::EIT_None;
}

FInventoryItem::~FInventoryItem()
{
}

FInventoryItem::FInventoryItem(const FInventoryItem& Item)
{
    ItemClass = Item.ItemClass;
    Quantity = Item.Quantity;
    IndexLocation = Item.IndexLocation;
    OwningInventory = Item.OwningInventory;
    ItemType = Item.ItemType;
}

void FInventoryItem::ClearItem()
{
    ItemClass = UBaseItem::StaticClass();
    Quantity = 0;
    ItemType = EItemType::EIT_None;
}
