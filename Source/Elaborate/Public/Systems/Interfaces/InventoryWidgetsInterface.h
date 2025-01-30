#pragma once


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryWidgetsInterface.generated.h"


class UInventorySlot;



UINTERFACE(MinimalAPI)
class UInventoryWidgetsInterface : public UInterface
{
	GENERATED_BODY()
};

class ELABORATE_API IInventoryWidgetsInterface
{
	GENERATED_BODY()


public:
	UFUNCTION()
	virtual void UpdateSlotUnderCursor(UInventorySlot* SlotUnderCursor) = 0;
};