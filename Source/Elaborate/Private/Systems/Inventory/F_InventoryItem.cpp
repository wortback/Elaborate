// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/F_InventoryItem.h"
#include "Systems/Inventory/CommonTypes.h"
#include "Systems/Inventory/Defines.h"





F_InventoryItem::F_InventoryItem()
{
    ItemClass = UBaseItem::StaticClass();
    Quantity = 0;
    IndexLocation = DEFAULT_INDEX_LOCATION;
    OwningInventory = nullptr;
    ItemType = EItemType::EIT_None;
}

F_InventoryItem::~F_InventoryItem()
{
}

F_InventoryItem::F_InventoryItem(const F_InventoryItem& Item)
{
    ItemClass = Item.ItemClass;
    Quantity = Item.Quantity;
    IndexLocation = Item.IndexLocation;
    OwningInventory = Item.OwningInventory;
    ItemType = Item.ItemType;
}

void F_InventoryItem::ClearItem()
{
    ItemClass = UBaseItem::StaticClass();
    Quantity = 0;
    ItemType = EItemType::EIT_None;
}
