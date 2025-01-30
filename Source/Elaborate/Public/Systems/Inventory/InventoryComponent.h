// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Systems/Inventory/FInventoryItem.h"
#include "InventoryComponent.generated.h"




/**
 * Actor Component that handles all operations related to the player's inventory.
 * 
 * Operations include:
 *		Adding/removing an item to/from the inventory
 *		Equipping/unequipping an item (only applicable for armour sets and weapons)
 *		Transferring items between actors that possess inventory components
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELABORATE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Array for items of types Armour, Weapon */
	TArray<FInventoryItem> Equipment;

	/** Array for items of type QuestItem */
	TArray<FInventoryItem> QuestItems;

	/** Array for items of type Consumable */
	TArray<FInventoryItem> Consumables;

	/** Array for items of type MiscellaneousItem */
	TArray<FInventoryItem> MiscellaneousItems;

	int32 EquipmentInitialCapacity = 20;

	int32 QuestItemsInitialCapacity = 30;

	int32 ConsumablesInitialCapacity = 15;

	int32 MiscellaneousItemsInitialCapacity = 30;

	/** Inventory item reserved for the equipped armour */
	FInventoryItem EquippedArmour;

	/** Inventory item reserved for the equipped weapon */
	FInventoryItem EquippedWeapon;

	/** Inventory item reserved for quick access, such as a hotkey or shortcut slot */
	FInventoryItem QuickAccessItem1;

	/** Inventory item reserved for quick access, such as a hotkey or shortcut slot */
	FInventoryItem QuickAccessItem2;

	/** Inventory item reserved for quick access, such as a hotkey or shortcut slot */
	FInventoryItem QuickAccessItem3;

	/** Inventory item reserved for quick access, such as a hotkey or shortcut slot */
	FInventoryItem QuickAccessItem4;

	int32 Gold = 0;

	float CarriedWeight = 0.0f;

	float MaxCarriedWeight = 100.f;

public:	
	/** Default Constructor */
	UInventoryComponent();

	/**
	 * Finds an available slot in the inventory to host an item
	 * 
	 * @param Item The item to be processed
	 * @return true if the item was successfully added
	 * @return false if the item could not be added
	 */
	bool ProcessItem(FInventoryItem* Item, int32 Quantity, bool UpdateWeight);

	/**
	 * Removes Item from the inventory
	 *
	 * @param Item The item to be removed
	 * @return true if the removal was successful
	 */
	bool RemoveItem(FInventoryItem* Item, int32 Quantity);

	/**
	 * Moves the Item from the inventory array to the equipped slot
	 * Only applicable to weapons and armour sets
	 *
	 * @param Item The item to be equipped
	 * @return true if the removal was successful
	 */
	bool EquipItem(FInventoryItem* Item, int32 Location = -1);

	/**
	 * Moves the Item from the equipped slot to the inventory
	 * Only applicable to weapons and armour sets
	 *
	 * @param Item The item to be unequipped
	 * @return true if the removal was successful
	 */
	bool UnequipItem(FInventoryItem* Item, int32 Location = -1);

	/**
	 * Moves the Item from this inventory to another one
	 *
	 * @param Item The item to be transferred
	 * @param Receiver The InvenotryComponent that receives the item
	 * @return true if the transfer was successful
	 */
	bool TransferItem(FInventoryItem* Item, UInventoryComponent* Receiver, int32 Quantity = 1);

	/**
	 * Moves the Item from the sender inventory to this inventory
	 *
	 * @param Item The item to be received
	 * @param Sender The InvenotryComponent that transfers the item
	 * @return true if the transfer was successful
	 */
	bool ReceiveItem(FInventoryItem* Item, UInventoryComponent* Sender, int32 Quantity = 1);

	/**
	 * Depending on ItemType returns the TArray the item can be assigned to
	 */
	const TArray<FInventoryItem>& GetItemsForItemType(EItemType ItemType) const;

	/**
	 * Depending on ItemType returns the TArray the item can be assigned to
	 */
	TArray<FInventoryItem>& SetItemsForItemType(EItemType ItemType);

protected:

	virtual void BeginPlay() override;

	
private:
	void InitialiseInventory();

	/**
	 * Initialises the array with empty inventory items of type EIT_None and ItemClass BaseItem
	 * 
	 * @param Index optional: start initialising the indices of the array items from the given index
	 */
	void InitialiseItemArray(TArray<FInventoryItem>& ItemArr, int32 Capacity, int32 Index = 0);

	void InitialiseEquippedItem(FInventoryItem& Item, int32 IndexLocation);

	FInventoryItem& GetSpecialItemByIndex(int32 Index);

	/**
	 * Once an item is added to the inventory, find an available slot to host the item
	 *
	 * @return available index location in the inventory
	 * @return DEFAULT_INDEX_LOCATION, if Item cannot be added
	 */
	int32 FindAvailableLocation(FInventoryItem* Item);

	/**
	 * Adds Item to the inventory
	 *
	 * @param Item The item to be added
	 * @param IndexLocation The index of the position in the inventory where the item is to be hosted
	 */
	FInventoryItem* AddItem(FInventoryItem* Item, int IndexLocation, int32 Quantity, bool UpdateWeight);

	void SwapEquipped(FInventoryItem& Item, FInventoryItem& EquippedItem);

	void EquipQAItem(FInventoryItem& Item, int32 Location);

	void Consume(FInventoryItem& Item);

	void UpdateCarriedWeight(float Weight);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE const TArray<FInventoryItem>& GetEquipment() const { return Equipment; }
	FORCEINLINE const TArray<FInventoryItem>& GetQuestItems() const { return QuestItems; }
	FORCEINLINE const TArray<FInventoryItem>& GetConsumables() const { return Consumables; }
	FORCEINLINE const TArray<FInventoryItem>& GetMiscellaneousItems() const { return MiscellaneousItems; }

	FORCEINLINE int32 GetEquipmentCapacity() const { return EquipmentInitialCapacity; }
	FORCEINLINE int32 GetQuestItemsCapacity() const { return QuestItemsInitialCapacity; }
	FORCEINLINE int32 GetConsumablesCapacity() const { return ConsumablesInitialCapacity; }
	FORCEINLINE int32 GetMiscellaneousItemsCapacity() const { return MiscellaneousItemsInitialCapacity; }

	FORCEINLINE const FInventoryItem& GetEquippedArmour() const { return EquippedArmour; }
	FORCEINLINE const FInventoryItem& GetEquippedWeapon() const { return EquippedWeapon; }

	FORCEINLINE float GetCarriedWeight() const { return CarriedWeight; };
	FORCEINLINE float GetMaxCarriedWeight() const { return MaxCarriedWeight; };
	FORCEINLINE int32 GetGold() const { return Gold; };

	FORCEINLINE bool IsEncumbered() const { return (MaxCarriedWeight >= CarriedWeight); };

	/** Returns a QA item based on its index
	* 
	* @param QAIndex Index of the quick item (range: 1-4)
	*/
	const FInventoryItem& GetQuickAccessItem(int32 QAIndex) const;
		
};
