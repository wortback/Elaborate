#pragma once


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryHUDInterface.generated.h"


class AActor;
class UInventoryComponent;
struct FInventoryItem;

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

	virtual void RemoveItem(FInventoryItem* Item, int32 Quantity) = 0;

	virtual bool ProcessItem(FInventoryItem* Item, int32 Quantity) = 0;

	virtual bool EquipItem(FInventoryItem* Item, int32 Location = -1) = 0;

	virtual bool UnequipItem(FInventoryItem* Item, int32 Location = -1) = 0;

	virtual bool SellItem(FInventoryItem* Item, int32 Quantity) = 0;

	virtual bool BuyItem(FInventoryItem* Item, int32 Quantity) = 0;

	virtual void UpdateNPCComponentPtr(UInventoryComponent* InventoryComp) = 0;

	virtual bool GetIsInTradeMode() const = 0;

	virtual void SetIsInTradeMode(bool bIsInTradeMode) = 0;
};