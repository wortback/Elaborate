// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/InventoryComponent.h"
#include "Systems/Interfaces/InventoryHUDInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Systems/Inventory/CommonTypes.h"
#include "Systems/Inventory//Defines.h"

// Logging
#include "Elaborate.h"





UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitialiseInventory();
}

bool UInventoryComponent::ProcessItem(F_InventoryItem* Item, int32 Quantity, bool UpdateWeight)
{
	if (Item)
	{
		if (Item->ItemType != EItemType::EIT_None)
		{
			int32 ItemIndexLocation = FindAvailableLocation(Item);

			UE_LOG(LogInventoryComponent, Log, TEXT("ItemType: %s"), *(EItemTypeToString(Item->ItemType)));

			if (ItemIndexLocation >= 0)
			{
				F_InventoryItem* AddedItem = AddItem(Item, ItemIndexLocation, Quantity, UpdateWeight);
				if (!AddedItem)
				{
					UE_LOG(LogInventoryComponent, Log, TEXT("Failed to process the item since the pointer is invalid."));
					delete AddedItem;
					return false;
				}
				else if (AddedItem->Quantity > 0)
				{
					return ProcessItem(AddedItem, AddedItem->Quantity, UpdateWeight);
				}
				else
				{
					UE_LOG(LogInventoryComponent, Warning, TEXT("Item is successfully added to the inventory!"));
					delete AddedItem;
					return true;
				}
			}
			else
			{
				UE_LOG(LogInventoryComponent, Warning, TEXT("Inventory is full"));
				return false;
			}
		}
	}
	else
	{
		UE_LOG(LogInventoryComponent, Log, TEXT("Failed to process the item since the pointer is invalid."));
	}
	return false;
}

void UInventoryComponent::UpdateCarriedWeight(float Weight)
{
	UE_LOG(LogInventoryComponent, Log, TEXT("Carried Weight is updated by %f"), Weight);
	CarriedWeight += Weight;
}

F_InventoryItem* UInventoryComponent::AddItem(F_InventoryItem* Item, int IndexLocation, int32 Quantity, bool UpdateWeight)
{
	UE_LOG(LogInventoryComponent, Log, TEXT("Adding a new item of type %s"), *(EItemTypeToString(Item->ItemType)));
	UE_LOG(LogInventoryComponent, Log, TEXT("In quantity %d"), Quantity);

	if (SetItemsForItemType(Item->ItemType).IsValidIndex(IndexLocation))
	{
		F_InventoryItem* ItemAtLocation = &SetItemsForItemType(Item->ItemType)[IndexLocation];

		// A new F_InventoryItem Object that stores DeltaQuantity
		// if it is bigger than 0, will attempt to store the remaining copies of the item in a separate slot
		F_InventoryItem* Result = new F_InventoryItem(*Item);

		UBaseItem* DefaultItem = Cast<UBaseItem>(Item->ItemClass->GetDefaultObject());
		if (DefaultItem)
		{
			int32 NewQuantity = ItemAtLocation->Quantity + Quantity;
			UE_LOG(LogInventoryComponent, Log, TEXT("NewQuantity %d"), NewQuantity);
			int32 QuantityClamped = FMath::Clamp(NewQuantity, 0, DefaultItem->StackSize);
			UE_LOG(LogInventoryComponent, Log, TEXT("QuantityClamped %d"), QuantityClamped);
			int32 DeltaQuantity = NewQuantity - FMath::Clamp(NewQuantity, 0, DefaultItem->StackSize);
			UE_LOG(LogInventoryComponent, Log, TEXT("DeltaQuantity %d"), DeltaQuantity);

			// Update member variables on the item st this particular slot
			ItemAtLocation->Quantity = QuantityClamped;
			ItemAtLocation->ItemClass = Item->ItemClass;
			ItemAtLocation->OwningInventory = this;
			ItemAtLocation->IndexLocation = IndexLocation;
			ItemAtLocation->ItemType = Item->ItemType;

			if (UpdateWeight)
			{
				UpdateCarriedWeight(DefaultItem->Weight * QuantityClamped);
			}
			

			Result->Quantity = DeltaQuantity;
		}
		

		return Result;
	}
	UE_LOG(LogInventoryComponent, Error, TEXT("[AddItem] Index location &d is out of bound."), IndexLocation);
	return nullptr;
}

bool UInventoryComponent::RemoveItem(F_InventoryItem* Item, int32 Quantity)
{
	UE_LOG(LogInventoryComponent, Log, TEXT("Removing %s items"), *FString::FromInt(Quantity));
	if (Item->ItemType != EItemType::EIT_None)
	{
		if (SetItemsForItemType(Item->ItemType).IsValidIndex(Item->IndexLocation))
		{
			float ItemWeight = 0.0f;
			F_InventoryItem* ItemAtLocation = &SetItemsForItemType(Item->ItemType)[Item->IndexLocation];
			if (ItemAtLocation->Quantity == Quantity)
			{
				UBaseItem* DefaultItem = Cast<UBaseItem>(ItemAtLocation->ItemClass->GetDefaultObject());
				ItemWeight = DefaultItem->Weight * ItemAtLocation->Quantity;
				ItemAtLocation->ItemClass = UBaseItem::StaticClass();
				ItemAtLocation->OwningInventory = nullptr;
				ItemAtLocation->Quantity = 0;
				ItemAtLocation->ItemType = EItemType::EIT_None;
			}
			else if (ItemAtLocation->Quantity > Quantity)
			{
				ItemAtLocation->Quantity -= Quantity;
			}
			else
			{
				UE_LOG(LogInventoryComponent, Error, TEXT("Attempting to remove more item instances than are present. Aborting the operation."));
				return false;
			}
			UpdateCarriedWeight(-ItemWeight);
			return true;
		}
		UE_LOG(LogInventoryComponent, Error, TEXT("[RemoveItem] Index location &d is out of bound."), Item->IndexLocation);
	}
	return false;
}

bool UInventoryComponent::EquipItem(F_InventoryItem* Item, int32 Location)
{
	switch (Item->ItemType)
	{
	case EItemType::EIT_Armour:
		UE_LOG(LogInventoryComponent, Log, TEXT("Equipping the armour set..."));
		SwapEquipped(*Item, EquippedArmour);
		return true;

	case EItemType::EIT_Weapon:
		UE_LOG(LogInventoryComponent, Log, TEXT("Equipping the weapon..."));
		SwapEquipped(*Item, EquippedWeapon);
		return true;
	case EItemType::EIT_Consumable:
		UE_LOG(LogInventoryComponent, Log, TEXT("Equipping the consumable..."));
		if (Location != -1)
		{
			EquipQAItem(*Item, Location);
		}
		else
		{
			Consume(*Item);
		}
		return true;
	case EItemType::EIT_None:
	case EItemType::EIT_Miscellaneous:
	case EItemType::EIT_Quest:
	default:
		UE_LOG(LogInventoryComponent, Warning, TEXT("This item cannot be equiped."));
		return false;
	}
}

bool UInventoryComponent::UnequipItem(F_InventoryItem* Item, int32 Location)
{
	UE_LOG(LogInventoryComponent, Log, TEXT("Unequipping the item..."));
	if (ProcessItem(Item, Item->Quantity, false))
	{
		if (Item->ItemType == EItemType::EIT_Armour)
		{
			EquippedArmour.ClearItem();
		}
		else if (Item->ItemType == EItemType::EIT_Weapon)
		{
			EquippedWeapon.ClearItem();
		}
		else if (Item->ItemType == EItemType::EIT_Consumable)
		{
			F_InventoryItem& ItemAtLocation = GetSpecialItemByIndex(Location);
			ItemAtLocation.ClearItem();
		}
	}
	return false;
}

bool UInventoryComponent::TransferItem(F_InventoryItem* Item, UInventoryComponent* Receiver, int32 Quantity /*= 1*/)
{
	if (Receiver->ProcessItem(Item, Quantity, true))
	{
		RemoveItem(Item, Quantity);
		return true;
	}
	UE_LOG(LogInventoryComponent, Error, TEXT("The item cannot be transferred."));
	return false;
}

bool UInventoryComponent::ReceiveItem(F_InventoryItem* Item, UInventoryComponent* Sender, int32 Quantity /*= 1*/)
{
	if (ProcessItem(Item, Quantity, true))
	{
		Sender->RemoveItem(Item, Quantity);
		return true;
	}
	UE_LOG(LogInventoryComponent, Error, TEXT("The item cannot be received."));
	return false;
}

void UInventoryComponent::SwapEquipped(F_InventoryItem& Item, F_InventoryItem& EquippedItem)
{
	if (!SetItemsForItemType(Item.ItemType).IsValidIndex(Item.IndexLocation))
	{
		UE_LOG(LogInventoryComponent, Error, TEXT("[SwapItem] Index location %d is out of bound."), Item.IndexLocation);
		return;
	}

	F_InventoryItem& ItemAtLocation = SetItemsForItemType(Item.ItemType)[Item.IndexLocation];
	F_InventoryItem Temp;
	Temp.ItemClass = EquippedItem.ItemClass;
	Temp.ItemType = EquippedItem.ItemType;
	Temp.Quantity = EquippedItem.Quantity;

	EquippedItem.ItemClass = ItemAtLocation.ItemClass;
	EquippedItem.ItemType = ItemAtLocation.ItemType;
	EquippedItem.Quantity = ItemAtLocation.Quantity;

	ItemAtLocation.ItemClass = Temp.ItemClass;
	ItemAtLocation.ItemType = Temp.ItemType;
	ItemAtLocation.Quantity = Temp.Quantity;
}

void UInventoryComponent::EquipQAItem(F_InventoryItem& Item, int32 Location)
{
	F_InventoryItem& EquippedItem = GetSpecialItemByIndex(Location);
	// Inventory to Special (empty) equip
	if (!IsQAItemIndex(Item.IndexLocation) && IsQAItemIndex(Location) && EquippedItem.ItemType == EItemType::EIT_None)
	{
		EquippedItem.ItemClass = Item.ItemClass;
		EquippedItem.ItemType = Item.ItemType;
		EquippedItem.Quantity = Item.Quantity;

		F_InventoryItem& ItemAtLocation = SetItemsForItemType(Item.ItemType)[Item.IndexLocation];
		ItemAtLocation.ClearItem();
	}
	// Inventory to Special (not empty) swap
	else if (!IsQAItemIndex(Item.IndexLocation) && IsQAItemIndex(Location) && EquippedItem.ItemType != EItemType::EIT_None)
	{
		SwapEquipped(Item, EquippedItem);
	}
	// Special to inventory (empty) unequip
	else if (IsQAItemIndex(Item.IndexLocation) && !IsQAItemIndex(Location) && EquippedItem.ItemType == EItemType::EIT_None)
	{
		UnequipItem(&Item, Location);
	}
	// Special to inventory (not empty)
	else if (IsQAItemIndex(Item.IndexLocation) && !IsQAItemIndex(Location) && EquippedItem.ItemType != EItemType::EIT_None)
	{
		SwapEquipped(EquippedItem, Item);
	}
	// Special to special
	else if (IsQAItemIndex(Item.IndexLocation) && IsQAItemIndex(Location))
	{
		F_InventoryItem Temp = EquippedItem;
		Temp.ItemClass = EquippedItem.ItemClass;
		Temp.ItemType = EquippedItem.ItemType;
		Temp.Quantity = EquippedItem.Quantity;

		EquippedItem.ItemClass = Item.ItemClass;
		EquippedItem.ItemType = Item.ItemType;
		EquippedItem.Quantity = Item.Quantity;

		F_InventoryItem& ItemAtLocation = GetSpecialItemByIndex(Item.IndexLocation);
		ItemAtLocation.ItemClass = Temp.ItemClass;
		ItemAtLocation.ItemType = Temp.ItemType;
		ItemAtLocation.Quantity = Temp.Quantity;
	}
	
}

void UInventoryComponent::Consume(F_InventoryItem& Item)
{
	// TODO: Actual implementation of the method
	UE_LOG(LogInventoryComponent, Warning, TEXT("Consuming the item..."));
	F_InventoryItem& ItemAtLocation = SetItemsForItemType(Item.ItemType)[Item.IndexLocation];
	ItemAtLocation.Quantity--;
	UBaseItem* DefaultItem = Cast<UBaseItem>(ItemAtLocation.ItemClass->GetDefaultObject());
	UpdateCarriedWeight(-DefaultItem->Weight);
}

const TArray<F_InventoryItem>& UInventoryComponent::GetItemsForItemType(EItemType ItemType) const
{
	switch (ItemType)
	{
	case EItemType::EIT_Armour:
	case EItemType::EIT_Weapon:
		return Equipment;
	case EItemType::EIT_Quest:
		return QuestItems;
	case EItemType::EIT_Consumable:
		return Consumables;
	case EItemType::EIT_Miscellaneous:
		return MiscellaneousItems;
	case EItemType::EIT_None:
	default:
		UE_LOG(LogInventoryComponent, Warning, TEXT("Cannot get the inventory section for an item since the item is of type EIT_None."));
		return Equipment;
	}
}

int32 UInventoryComponent::FindAvailableLocation(F_InventoryItem* Item)
{
	UE_LOG(LogInventoryComponent, Log, TEXT("Searching for an available location..."));

	// Case 1: There's another Item of the same class present in the inventory and it's quantity is less than the stack size
	UBaseItem* DefaultItem = Cast<UBaseItem>(Item->ItemClass->GetDefaultObject(true));
	if (DefaultItem->bIsStackable)
	{
		for (const F_InventoryItem& Element : GetItemsForItemType(Item->ItemType))
		{
			if (Element.ItemClass == Item->ItemClass)
			{
				if (Element.Quantity < DefaultItem->StackSize)
				{
					UE_LOG(LogInventoryComponent, Log, TEXT("Found an item of the same class at location %d"), Element.IndexLocation);
					return Element.IndexLocation;
				}
			}
		}
	}

	// Case 2: If there's an inventory slot that is currently empty
	for (const F_InventoryItem& Element : GetItemsForItemType(Item->ItemType))
	{
		if (Element.ItemClass == UBaseItem::StaticClass())
		{
			UE_LOG(LogInventoryComponent, Log, TEXT("Found an empty slot at location %d"), Element.IndexLocation);
			return Element.IndexLocation;
		}
	}

	// Case 3:Run out of inventory slots => extend the inventory capacity
	UE_LOG(LogInventoryComponent, Warning, TEXT("No suitable slot found."));

	int32 Capacity = GetItemsForItemType(Item->ItemType).Num();
	TArray<F_InventoryItem>& Items = SetItemsForItemType(Item->ItemType);
	UE_LOG(LogInventoryComponent, Log, TEXT("Extending current inventory capacity."));
	UE_LOG(LogInventoryComponent, Log, TEXT("Current capacity is: %d"), Capacity);
	UE_LOG(LogInventoryComponent, Log, TEXT("New capacity is: %d"), Capacity + CAPACITY_INCREASE_FACTOR);
	TArray<F_InventoryItem> TempEmpty;
	InitialiseItemArray(TempEmpty, CAPACITY_INCREASE_FACTOR, Capacity);
	Items.Append(TempEmpty);

	return FindAvailableLocation(Item);
}

TArray<F_InventoryItem>& UInventoryComponent::SetItemsForItemType(EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::EIT_Armour:
	case EItemType::EIT_Weapon:
		return Equipment;
	case EItemType::EIT_Quest:
		return QuestItems;
	case EItemType::EIT_Consumable:
		return Consumables;
	case EItemType::EIT_Miscellaneous:
		return MiscellaneousItems;
	case EItemType::EIT_None:
	default:
		UE_LOG(LogInventoryComponent, Error, TEXT("Cannot get the inventory section for an item since the item is of type EIT_None."));
		return Equipment;
	}
}

void UInventoryComponent::InitialiseInventory()
{
	// Initialise all arrays
	TArray<TArray<F_InventoryItem>*> ItemArrays = { &Equipment, &QuestItems, &Consumables, &MiscellaneousItems };
	TArray<int32> Capacities = { EquipmentInitialCapacity, QuestItemsInitialCapacity, ConsumablesInitialCapacity, MiscellaneousItemsInitialCapacity };
	for (int32 i = 0; i < ItemArrays.Num(); ++i)
	{
		InitialiseItemArray(*ItemArrays[i], Capacities[i]);
	}

	// Initialise Equipped Items (special items)
	TArray<F_InventoryItem*> EquippedItems = { &EquippedArmour, &EquippedWeapon,
		&QuickAccessItem1, &QuickAccessItem2, &QuickAccessItem3, &QuickAccessItem4 };
	TArray<int32> SpecialIndexLocations = { EQ_ARMOUR_INDEX_LOCATION, EQ_WEAPON_INDEX_LOCATION, QUICK_ITEM_1_INDEX_LOCATION,
											QUICK_ITEM_2_INDEX_LOCATION, QUICK_ITEM_3_INDEX_LOCATION, QUICK_ITEM_4_INDEX_LOCATION };

	for (int32 i = 0; i < EquippedItems.Num(); ++i)
	{
		InitialiseEquippedItem(*EquippedItems[i], SpecialIndexLocations[i]);
	}

}

void UInventoryComponent::InitialiseItemArray(TArray<F_InventoryItem>& ItemArr, int32 Capacity, int32 Index)
{
	ItemArr.SetNum(Capacity);
	for (int32 i = 0; i < Capacity; ++i)
	{
		ItemArr[i].ItemClass = UBaseItem::StaticClass();
		ItemArr[i].Quantity = 0;
		ItemArr[i].OwningInventory = this;
		ItemArr[i].IndexLocation = i + Index;
		ItemArr[i].ItemType = EItemType::EIT_None;
	}
}

void UInventoryComponent::InitialiseEquippedItem(F_InventoryItem& Item, int32 IndexLocation)
{
	Item.Quantity = 0;
	Item.OwningInventory = this;
	Item.IndexLocation = IndexLocation;
	Item.ItemClass = UBaseItem::StaticClass();
	Item.ItemType = EItemType::EIT_None;
}

F_InventoryItem& UInventoryComponent::GetSpecialItemByIndex(int32 Index)
{
	switch (Index)
	{
	case EQ_ARMOUR_INDEX_LOCATION:
		return EquippedArmour;
	case EQ_WEAPON_INDEX_LOCATION:
		return EquippedWeapon;
	case QUICK_ITEM_1_INDEX_LOCATION:
		return QuickAccessItem1;
	case QUICK_ITEM_2_INDEX_LOCATION:
		return QuickAccessItem2;
	case QUICK_ITEM_3_INDEX_LOCATION:
		return QuickAccessItem3;
	case QUICK_ITEM_4_INDEX_LOCATION:
		return QuickAccessItem4;
	default:
		UE_LOG(LogInventoryComponent, Error, TEXT("Index %d"), Index);
		UE_LOG(LogInventoryComponent, Error, TEXT("Provided index does not match any special item index!"));
		return EquippedArmour;
	}
}




// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

const F_InventoryItem& UInventoryComponent::GetQuickAccessItem(int32 QAIndex) const
{
	switch (QAIndex)
	{
	default:
	case 1:
		return QuickAccessItem1;
	case 2:
		return QuickAccessItem2;
	case 3:
		return QuickAccessItem3;
	case 4:
		return QuickAccessItem4;
	}
}
