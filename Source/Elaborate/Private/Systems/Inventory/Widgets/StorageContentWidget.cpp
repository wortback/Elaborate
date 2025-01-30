// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/StorageContentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Inventory/BaseStorage.h"
#include "Systems/Inventory/Widgets/StorageContentSlot.h"
#include "Systems/Inventory/FInventoryItem.h"
#include "Systems/Inventory/BaseItem.h"
#include "Systems/Inventory/BaseConsumable.h"
#include "Systems/Inventory/CommonTypes.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Systems/Inventory/Widgets/ItemInfoWidget.h"

// Logging
#include "Elaborate.h"




void UStorageContentWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UStorageContentWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		if (PlayerController->bShowMouseCursor)
		{
			SetFocus();
		}
	}
}

FReply UStorageContentWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey().GetFName() == "Tab" || InKeyEvent.GetKey().GetFName() == "Escape")
	{
		if (OwningStorage)
		{
			OwningStorage->HideContent();
		}
		else
		{
			UE_LOG(LogInventoryHUD, Warning, TEXT("Owning storage is NULL for the ContentWidget."));
		}
	}
	return FReply::Handled();

}

void UStorageContentWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// If all slots are removed (all items taken), remove the widget
	if (!SlotBox->HasAnyChildren())
	{
		OwningStorage->HideContent();
	}
}

void UStorageContentWidget::CreateInfoWidget(FInventoryItem Item)
{
	if (InfoWidgetClass && Item.ItemType != EItemType::EIT_None)
	{
		InfoWidget = CreateWidget<UItemInfoWidget>(GetWorld(), InfoWidgetClass);
		if (InfoWidget)
		{
			InfoWidget->SetInspectedItem(Item);
			double CursorX = 0.0;
			double CursorY = 0.0;
			APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PlayerController)
			{
				PlayerController->GetMousePosition(CursorX, CursorY);
			}
			InfoWidget->SetPositionInViewport(FVector2D(CursorX, CursorY));
			InfoWidget->AddToViewport();
		}

	}
}


