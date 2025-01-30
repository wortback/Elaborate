// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/InventoryWidget.h"
#include "Components/TextBlock.h"
#include "Systems/Inventory/Widgets/TabButtonWidget.h"
#include "Systems/Inventory/Widgets/PlayerInventoryWindow.h"
#include "Systems/Inventory/Defines.h"
#include "Systems/Inventory/Widgets/InventorySlot.h"

//Logging
#include "Elaborate.h"



void UInventoryWidget::UpdateMenu()
{
	PlayerInventoryWindow->UpdateMenu();


}

void UInventoryWidget::UpdateCarriedWeight(float CurrentWeight, float MaxWeight)
{
	int DisplayedWeight = (CurrentWeight - (int)CurrentWeight > 0.5f) ? ++CurrentWeight : CurrentWeight;
	FString temp = FString::FromInt(DisplayedWeight) + "/" + FString::FromInt((int)MaxWeight);
	CarriedWeight->SetText(FText::FromString(temp));
}

void UInventoryWidget::UpdateGold(int PlayerGold)
{
	Gold->SetText(FText::FromString(FString::FromInt(PlayerGold)));
}

bool UInventoryWidget::CanDropOnSpecialSlot(UInventorySlot* UnderDragSlot, UInventorySlot* DraggedSlot)
{
	if (UnderDragSlot->Item.IndexLocation == EQ_ARMOUR_INDEX_LOCATION && DraggedSlot->Item.ItemType == EItemType::EIT_Armour)
	{
		return true;
	}
	else if (UnderDragSlot->Item.IndexLocation == EQ_WEAPON_INDEX_LOCATION && DraggedSlot->Item.ItemType == EItemType::EIT_Weapon)
	{
		return true;
	}
	else if (IsQAItemIndex(UnderDragSlot->Item.IndexLocation) && DraggedSlot->Item.ItemType == EItemType::EIT_Consumable)
	{
		return true;
	}
	return false;
}
