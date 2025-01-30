// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Systems/Interfaces/InventoryWidgetsInterface.h"
#include "InventorySlotDragDropOperation.generated.h"


class UInventorySlot;
/**
 * 
 */
UCLASS()
class ELABORATE_API UInventorySlotDragDropOperation : public UDragDropOperation, public IInventoryWidgetsInterface
{
	GENERATED_BODY()
	
	/** Slot that is located under the dragged slot */
	UInventorySlot* UnderDragSlot = nullptr;

public:
	UInventorySlotDragDropOperation(const FObjectInitializer& ObjectInitializer);

	virtual void UpdateSlotUnderCursor(UInventorySlot* SlotUnderCursor) override;

	virtual void Drop(const FPointerEvent& PointerEvent);

	virtual void DragCancelled(const FPointerEvent& PointerEvent);

	virtual void Dragged(const FPointerEvent& PointerEvent);

};
