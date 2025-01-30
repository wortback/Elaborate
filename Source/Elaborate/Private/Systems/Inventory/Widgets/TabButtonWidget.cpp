// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/TabButtonWidget.h"
#include "Systems/Inventory/Widgets/BaseButton.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Interfaces/InventoryHUDInterface.h"
#include "Systems/Inventory/Widgets/InventoryWidget.h"
#include "GameFramework/Character.h"

// Logging
#include "Elaborate.h"



void UTabButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EquipmentTabButton->OnPressed.AddDynamic(this, &UTabButtonWidget::OnEquipmentTabPressed);
	QuestTabButton->OnPressed.AddDynamic(this, &UTabButtonWidget::OnQuestTabPressed);
	ConsumableTabButton->OnPressed.AddDynamic(this, &UTabButtonWidget::OnConsumableTabPressed);
	MiscTabButton->OnPressed.AddDynamic(this, &UTabButtonWidget::OnMiscTabPressed);
}

void UTabButtonWidget::OnEquipmentTabPressed()
{
	SetCurrentTab(EInventoryWidgetTab::EIWT_Equipment);
	UpdateInventoryHUD();
}

void UTabButtonWidget::OnQuestTabPressed()
{
	SetCurrentTab(EInventoryWidgetTab::EIWT_Quest);
	UpdateInventoryHUD();
}

void UTabButtonWidget::OnConsumableTabPressed()
{

	SetCurrentTab(EInventoryWidgetTab::EIWT_Consumable);
	UpdateInventoryHUD();
}

void UTabButtonWidget::OnMiscTabPressed()
{
 	SetCurrentTab(EInventoryWidgetTab::EIWT_Miscellaneous);
	UpdateInventoryHUD();

}

void UTabButtonWidget::UpdateInventoryHUD()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		IInventoryHUDInterface* Interface = Cast<IInventoryHUDInterface>(PlayerCharacter);
		if (Interface)
		{
			//UE_LOG(LogInventoryHUD, Log, TEXT("Interface Cast to IInventoryHUDInterface successful"));
			Interface->UpdateInventoryHUD();
		}
// 		else
// 		{
// 			UE_LOG(LogInventoryHUD, Warning, TEXT("Interface Cast to IInventoryHUDInterface unsuccessful"));
// 		}
	}
}
