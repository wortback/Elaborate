// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/InventorySlotDragDropOperation.h"
#include "Systems/Inventory/Widgets/InventorySlot.h"
#include "Systems/Inventory/Widgets/PrimaryHUDWidget.h"

// Logging
#include "Elaborate.h"



UInventorySlotDragDropOperation::UInventorySlotDragDropOperation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (UnderDragSlot)
	{
		UE_LOG(LogInventoryHUD, Warning, TEXT("SLOT"));
	}
}

void UInventorySlotDragDropOperation::UpdateSlotUnderCursor(UInventorySlot* SlotUnderCursor)
{
	UnderDragSlot = SlotUnderCursor;
}

void UInventorySlotDragDropOperation::Drop(const FPointerEvent& PointerEvent)
{
	UE_LOG(LogInventoryHUD, Log, TEXT("Item is dropped."));

	if (UnderDragSlot)
	{
		if (UnderDragSlot != Payload)
		{
			UInventorySlot* DraggedSlot = Cast<UInventorySlot>(Payload);
			if (DraggedSlot)
			{
				UnderDragSlot->OwningHUD->OnItemDrop(UnderDragSlot, DraggedSlot);
			}
		}
	}
}

void UInventorySlotDragDropOperation::DragCancelled(const FPointerEvent& PointerEvent)
{
	
}

void UInventorySlotDragDropOperation::Dragged(const FPointerEvent& PointerEvent)
{
	UE_LOG(LogInventoryHUD, Log, TEXT("Is dragging..."));
}
