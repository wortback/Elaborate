// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/PlayerInventoryWindow.h"
#include "Components/TextBlock.h"
#include "Systems/Inventory/Widgets/TabButtonWidget.h"

//Logging
#include "Elaborate.h"



EItemType UPlayerInventoryWindow::GetItemTypeForCurrentTab()
{
	switch (GetCurrentTab())
	{
	case EInventoryWidgetTab::EIWT_Equipment:
		return EItemType::EIT_Armour;
		break;
	case EInventoryWidgetTab::EIWT_Quest:
		return EItemType::EIT_Quest;
		break;
	case EInventoryWidgetTab::EIWT_Consumable:
		return EItemType::EIT_Consumable;
		break;
	case EInventoryWidgetTab::EIWT_Miscellaneous:
		return EItemType::EIT_Miscellaneous;
		break;
	default:
		UE_LOG(LogInventoryHUD, Error, TEXT("Cannot get ItemType. Invalid current tab!"));
		return EItemType::EIT_None;
		break;
	}
}

void UPlayerInventoryWindow::UpdateMenu()
{
	switch (GetCurrentTab())
	{
	case EInventoryWidgetTab::EIWT_Equipment:
		TabName->SetText(FText::FromString("EQUIPMENT"));
		break;
	case EInventoryWidgetTab::EIWT_Quest:
		TabName->SetText(FText::FromString("QUEST ITEMS"));
		break;
	case EInventoryWidgetTab::EIWT_Consumable:
		TabName->SetText(FText::FromString("CONSUMABLES"));
		break;
	case EInventoryWidgetTab::EIWT_Miscellaneous:
		TabName->SetText(FText::FromString("MISC"));
		break;
	default:
		UE_LOG(LogInventoryHUD, Error, TEXT("Cannot update menu! Invalid current tab!"));
		break;
	}
}

void UPlayerInventoryWindow::NativeConstruct()
{
	Super::NativeConstruct();
	TabName->SetText(FText::FromString("EQUIPMENT"));
	UpdateMenu();
}

EInventoryWidgetTab UPlayerInventoryWindow::GetCurrentTab() const
{
	return TabSwitch->GetCurrentTab();
}

void UPlayerInventoryWindow::SetCurrentTab(EInventoryWidgetTab Tab)
{
	TabSwitch->SetCurrentTab(Tab);
}

void UPlayerInventoryWindow::SetCurrentTabByItemType(EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::EIT_Armour:
	case EItemType::EIT_Weapon:
		SetCurrentTab(EInventoryWidgetTab::EIWT_Equipment);
		break;
	case EItemType::EIT_Quest:
		SetCurrentTab(EInventoryWidgetTab::EIWT_Quest);
		break;
	case EItemType::EIT_Consumable:
		SetCurrentTab(EInventoryWidgetTab::EIWT_Consumable);
		break;
	case EItemType::EIT_Miscellaneous:
		SetCurrentTab(EInventoryWidgetTab::EIWT_Miscellaneous);
		break;
	case EItemType::EIT_None:
	default:
		UE_LOG(LogInventoryHUD, Error, TEXT("[SetCurrentTabByItemType] No inventory tab available for the provided item type."));
		break;
	}

	UpdateMenu();
}
