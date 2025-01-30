// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/StorageContentSlot.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/TextBlock.h"
#include "Systems/Inventory/BaseItem.h"
#include "Components/Button.h"
#include "Systems/Interfaces/InventoryHUDInterface.h"
#include "Systems/Inventory/Widgets/ItemInfoWidget.h"
#include "Systems/Inventory/Widgets/StorageContentWidget.h"

// Logging
#include "Elaborate.h"


//TODO display the amount of instances of the item in the slot
void UStorageContentSlot::NativeConstruct()
{
	UseButton->OnPressed.AddDynamic(this, &UStorageContentSlot::OnButtonClicked);

	UBaseItem* BaseItem = Cast<UBaseItem>(SlotItem->ItemClass->GetDefaultObject(true));
	if (BaseItem)
	{
		// Formats the item name in the following way: "[EItemType] ItemName"
		FString Name = BaseItem->ItemName.ToString();
		SlotText->SetText(FText::FromString(("[") + EItemTypeToString(SlotItem->ItemType) + FString("] ") + Name));
	}
}

void UStorageContentSlot::OnButtonClicked()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		IInventoryHUDInterface* Interface = Cast<IInventoryHUDInterface>(PlayerCharacter);
		if (Interface)
		{
			// TODO allow to take a desired amount of items from one slot
			if (Interface->ProcessItem(SlotItem, SlotItem->Quantity))
			{
				SlotItem->ClearItem();
				UE_LOG(LogInventoryHUD, Log, TEXT("Successfully collected the item from the storage."));
				RemoveFromParent();
			}	
		}
	}
}

void UStorageContentSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UStorageContentSlot::BroadcastItem, 2.0f, false);
}

void UStorageContentSlot::BroadcastItem()
{
	if (SlotItem)
	{
		FOnSlotEnteredDelegate.Broadcast(*SlotItem);
	}
}

void UStorageContentSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}
