#pragma once


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryHUDInterface.generated.h"


class AActor;
class UInventoryComponent;
struct F_InventoryItem;

/** Interface for a playable character that possesses an inventory component */
UINTERFACE(MinimalAPI)
class UInventoryHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class ELABORATE_API IInventoryHUDInterface
{
	GENERATED_BODY()
	

public:
	virtual AActor* LookAt() = 0;

	virtual UInventoryComponent* GetInventoryComponent() const = 0;

	virtual void OpenPlayerInventory() = 0;

	virtual void OpenNPCInventory(UInventoryComponent* NPCInventoryComponent) = 0;

	virtual void UpdateInventoryHUD() = 0;

	virtual void RemoveItem(F_InventoryItem* Item, int32 Quantity) = 0;

	virtual bool ProcessItem(F_InventoryItem* Item, int32 Quantity) = 0;

	virtual bool EquipItem(F_InventoryItem* Item, int32 Location = -1) = 0;

	virtual bool UnequipItem(F_InventoryItem* Item, int32 Location = -1) = 0;

	virtual bool SellItem(F_InventoryItem* Item, int32 Quantity) = 0;

	virtual bool BuyItem(F_InventoryItem* Item, int32 Quantity) = 0;

	virtual void UpdateNPCComponentPtr(UInventoryComponent* InventoryComp) = 0;

	virtual bool GetIsInTradeMode() const = 0;

	virtual void SetIsInTradeMode(bool bIsInTradeMode) = 0;
};