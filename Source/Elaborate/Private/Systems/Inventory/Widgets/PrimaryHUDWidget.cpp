// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/PrimaryHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Systems/Inventory/Widgets/InventorySlot.h"
#include "Components/WrapBox.h"
#include "Systems/Inventory/InventoryComponent.h"
#include "Systems/Inventory/Widgets/PlayerInventoryWindow.h"
#include "Systems/Inventory/Widgets/TransferItemsWidget.h"
#include "Systems/Inventory/Widgets/ItemInfoWidget.h"
#include "Systems/Inventory/Defines.h"

// Logging
#include "Elaborate.h"



void UPrimaryHUDWidget::ShowPlayerInventory(bool bShowInventory)
{
	if (bShowInventory)
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPrimaryHUDWidget::ShowNPCInventory(bool bShowInventory)
{
	if (bShowInventory)
	{
		PlayerInventory->SetVisibility(ESlateVisibility::Visible);
		NPCInventory->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PlayerInventory->SetVisibility(ESlateVisibility::Collapsed);
		NPCInventory->SetVisibility(ESlateVisibility::Collapsed);

		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (Character)
		{
			IInventoryHUDInterface* Interface = Cast<IInventoryHUDInterface>(Character);
			if (Interface)
			{
				Interface->UpdateNPCComponentPtr(nullptr);
			}
		}
	}
}

void UPrimaryHUDWidget::UpdateInventory(const UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent && InventorySlotClassShort && InventorySlotClassLong)
	{
		InventoryMenu->UpdateMenu();
		InventoryMenu->UpdateGold(InventoryComponent->GetGold());
		InventoryMenu->UpdateCarriedWeight(InventoryComponent->GetCarriedWeight(), InventoryComponent->GetMaxCarriedWeight());

		UE_LOG(LogInventoryHUD, Log, TEXT("Current tab is %d"), InventoryMenu->PlayerInventoryWindow->GetItemTypeForCurrentTab());
		InventoryMenu->PlayerInventoryWindow->InventoryBox->ClearChildren();
		
		// Check what slot type to use
		TSubclassOf<UInventorySlot> SlotClass;
		if (InventoryMenu->PlayerInventoryWindow->GetItemTypeForCurrentTab() == EItemType::EIT_Armour ||
			InventoryMenu->PlayerInventoryWindow->GetItemTypeForCurrentTab() == EItemType::EIT_Weapon)
		{
			SlotClass = InventorySlotClassLong;
		}
		else
		{
			SlotClass = InventorySlotClassShort;
		}

		for (const FInventoryItem& Item : InventoryComponent->GetItemsForItemType(InventoryMenu->PlayerInventoryWindow->GetItemTypeForCurrentTab()))
		{
			UInventorySlot* ChildSlot = CreateWidget<UInventorySlot>(GetWorld(), SlotClass);
			ChildSlot->Item = Item;
			ChildSlot->OwningHUD = this;
			InventoryMenu->PlayerInventoryWindow->InventoryBox->AddChildToWrapBox(ChildSlot);
		}

		// Update special slots
		TArray<TObjectPtr<UInventorySlot>> EquippedSlots = { InventoryMenu->EquippedArmour, InventoryMenu->EquippedWeapon };
		TArray<TObjectPtr<UInventorySlot>> QASlots = { InventoryMenu->QuickSlot1, InventoryMenu->QuickSlot2, 
													   InventoryMenu->QuickSlot3, InventoryMenu->QuickSlot4 };
		TArray<const FInventoryItem*> EquippedItems = { &InventoryComponent->GetEquippedArmour(), &InventoryComponent->GetEquippedWeapon() };
		for (int32 i = 0; i < EquippedSlots.Num(); ++i)
		{
			UpdateEquippedAndQASlots(EquippedSlots[i], EquippedItems[i]);
		}
		for (int32 i = 0; i < QASlots.Num(); ++i)
		{
			UpdateEquippedAndQASlots(QASlots[i], &InventoryComponent->GetQuickAccessItem(i+1));
		}

	}
	else
	{
		UE_LOG(LogInventoryHUD, Warning, TEXT("Cannot update the inventory. InventoryComponent is NULL or InventorySlotClass is not set in PrimaryHUD."));
	}
}

void UPrimaryHUDWidget::UpdateEquippedAndQASlots(TObjectPtr<UInventorySlot> InventorySlot, const FInventoryItem* Item)
{
	InventorySlot->Item = *Item;
	InventorySlot->OwningHUD = this;
	InventorySlot->UpdateSlotContent();
}

void UPrimaryHUDWidget::UpdateInventory(const UInventoryComponent* PlayerComp, const UInventoryComponent* NPCComp)
{
	if (PlayerComp && NPCComp && InventorySlotClassShort)
	{
		PlayerInventory->UpdateMenu();
		NPCInventory->UpdateMenu();
		PlayerInventory->InventoryBox->ClearChildren();
		NPCInventory->InventoryBox->ClearChildren();

		// Check what slot type to use
		TSubclassOf<UInventorySlot> SlotClass;
		if (PlayerInventory->GetItemTypeForCurrentTab() == EItemType::EIT_Armour ||
			PlayerInventory->GetItemTypeForCurrentTab() == EItemType::EIT_Weapon)
		{
			SlotClass = InventorySlotClassLong;
		}
		else
		{
			SlotClass = InventorySlotClassShort;
		}

		for (const FInventoryItem& Item : PlayerComp->GetItemsForItemType(PlayerInventory->GetItemTypeForCurrentTab()))
		{
			UInventorySlot* ChildSlot = CreateWidget<UInventorySlot>(GetWorld(), SlotClass);
			ChildSlot->Item = Item;
			ChildSlot->OwningHUD = this;
			PlayerInventory->InventoryBox->AddChildToWrapBox(ChildSlot);
		}

		if (PlayerInventory->GetItemTypeForCurrentTab() == EItemType::EIT_Armour ||
			PlayerInventory->GetItemTypeForCurrentTab() == EItemType::EIT_Weapon)
		{
			SlotClass = InventorySlotClassLong;
		}
		else
		{
			SlotClass = InventorySlotClassShort;
		}

		for (const FInventoryItem& Item : NPCComp->GetItemsForItemType(NPCInventory->GetItemTypeForCurrentTab()))
		{
			UInventorySlot* ChildSlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClassShort);
			ChildSlot->Item = Item;
			ChildSlot->OwningHUD = this;
			NPCInventory->InventoryBox->AddChildToWrapBox(ChildSlot);
		}
	}
	else
	{
		UE_LOG(LogInventoryHUD, Warning, TEXT("Cannot update the inventory. Either PlayerComp or NPCComp is NULL or InventorySlotClass is not set in PrimaryHUD."));
	}
}

void UPrimaryHUDWidget::CloseInventory()
{
	// Don't do anything, if the widget is collapsed
	if (InventoryMenu->GetVisibility() == ESlateVisibility::Visible)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;

			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}

		ShowPlayerInventory(false);
	}

	if (NPCInventory->GetVisibility() == ESlateVisibility::Visible)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;

			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}

		ShowNPCInventory(false);
	}
}

FReply UPrimaryHUDWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FName Key = InKeyEvent.GetKey().GetFName();
	if (Key == "Tab" || Key == "Escape")
    {
		CloseInventory();
    }
	
	else if (Key == "D" || Key == "E")
	{
		if (OverSlot)
		{
			ExecuteKeyBinding(Key);
		}
	}
	return FReply::Handled();
}

void UPrimaryHUDWidget::ExecuteKeyBinding(FName Key)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		IInventoryHUDInterface* Interface = Cast<IInventoryHUDInterface>(PlayerCharacter);
		if (Interface)
		{
			if (Key == "D")
			{
				Interface->RemoveItem(&(OverSlot->Item), 1);
			}
			else if (Key == "E")
			{
				ExecuteEKey(Interface);
			}
			Interface->UpdateInventoryHUD();
		}
	}
}

void UPrimaryHUDWidget::ExecuteEKey(IInventoryHUDInterface* Interface)
{
	// If the character is not trading:
	if (!Interface->GetIsInTradeMode())
	{
		if (IsSpecialItemIndex(OverSlot->Item.IndexLocation))
		{
			Interface->UnequipItem(&(OverSlot->Item), OverSlot->Item.IndexLocation);
			InventoryMenu->PlayerInventoryWindow->SetCurrentTabByItemType(OverSlot->Item.ItemType);
		}
		else
		{
			Interface->EquipItem(&(OverSlot->Item));
		}
	}
	// If the character is trading:
	else
	{
		if (OverSlot->Item.Quantity < MIN_REQUIRED_FOR_SLIDER)
		{
			// Is this item located in the character's inventory?
			if (OverSlot->Item.OwningInventory == Interface->GetInventoryComponent())
			{
				Interface->SellItem(&(OverSlot->Item), 1);
				NPCInventory->SetCurrentTabByItemType(OverSlot->Item.ItemType);
			}
			// Or NPC's?
			else
			{
				Interface->BuyItem(&(OverSlot->Item), 1);
				PlayerInventory->SetCurrentTabByItemType(OverSlot->Item.ItemType);
			}
		}
		else
		{
			CreateTransferWidget(OverSlot);
		}	
	}
}

void UPrimaryHUDWidget::HandleTransferValueConfirmed(int32 TransferValue)
{

	ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		IInventoryHUDInterface* Interface = Cast<IInventoryHUDInterface>(PlayerCharacter);
		if (Interface)
		{
			// Is this item located in the character's inventory?
			if (TransferItemsWidget->GetItemSlot()->Item.OwningInventory == Interface->GetInventoryComponent())
			{
				Interface->SellItem(&(TransferItemsWidget->GetItemSlot()->Item), TransferValue);
				NPCInventory->SetCurrentTabByItemType(TransferItemsWidget->GetItemSlot()->Item.ItemType);
			}
			// Or NPC's?
			else
			{
				Interface->BuyItem(&(TransferItemsWidget->GetItemSlot()->Item), TransferValue);
				PlayerInventory->SetCurrentTabByItemType(TransferItemsWidget->GetItemSlot()->Item.ItemType);
			}

			Interface->UpdateInventoryHUD();
		}
	}

	// Set focus on the primary HUD again (TODO: It's not working)
	SetFocus();
}

void UPrimaryHUDWidget::CreateTransferWidget(UInventorySlot* ItemSlot)
{
	if (TransferItemsWidgetClass)
	{
		TransferItemsWidget = CreateWidget<UTransferItemsWidget>(GetWorld(), TransferItemsWidgetClass);
		if (TransferItemsWidget)
		{
			TransferItemsWidget->FOnSliderValueConfirmedDelegate.AddDynamic(this, &UPrimaryHUDWidget::HandleTransferValueConfirmed);
			TransferItemsWidget->SetMaxValue(ItemSlot->Item.Quantity);
			TransferItemsWidget->SetItemSlot(ItemSlot);
			TransferItemsWidget->SetPositionInViewport(FVector2D(500, 500));
			TransferItemsWidget->AddToViewport();
			TransferItemsWidget->SetFocus();
		}
	}
}

void UPrimaryHUDWidget::DisplayInfoWidget()
{
	if (OverSlot)
	{
		if (InfoWidgetClass && OverSlot->Item.ItemType != EItemType::EIT_None)
		{
			InfoWidget = CreateWidget<UItemInfoWidget>(GetWorld(), InfoWidgetClass);
			if (InfoWidget)
			{
				InfoWidget->SetInspectedItem(OverSlot->Item);
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
}

void UPrimaryHUDWidget::DestroyInfoWidget()
{
	if (InfoWidget)
	{
		InfoWidget->RemoveFromParent();
	}
}

void UPrimaryHUDWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (!TransferItemsWidget)
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
}

void UPrimaryHUDWidget::UpdateSlotUnderCursor(UInventorySlot* SlotUnderCursor)
{
	OverSlot = SlotUnderCursor;
	if (TransferItemsWidget)
	{
		TransferItemsWidget->RemoveFromParent();
	}	
}

void UPrimaryHUDWidget::UpdateHUD()
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		IInventoryHUDInterface* Interface = Cast<IInventoryHUDInterface>(PlayerCharacter);
		if (Interface)
		{
			Interface->UpdateInventoryHUD();
		}
	}
}

void UPrimaryHUDWidget::OnItemDrop(UInventorySlot* UnderDragSlot, UInventorySlot* DragSlot)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		IInventoryHUDInterface* Interface = Cast<IInventoryHUDInterface>(PlayerCharacter);
		if (Interface)
		{
			// Is the item dropped on special slot? 
			if (IsSpecialItemIndex(UnderDragSlot->Item.IndexLocation))
			{
				// Check if the item is dropped on a special slot or not
				if (InventoryMenu->CanDropOnSpecialSlot(UnderDragSlot, DragSlot))
				{
					Interface->EquipItem(&(DragSlot->Item), UnderDragSlot->Item.IndexLocation);
				}
			}
			// If the item dragged from the special slot back to the inventory? 
			else if (IsSpecialItemIndex(DragSlot->Item.IndexLocation))
			{
				Interface->UnequipItem(&(DragSlot->Item), DragSlot->Item.IndexLocation);
				InventoryMenu->PlayerInventoryWindow->SetCurrentTabByItemType(DragSlot->Item.ItemType);
			}
			else
			{
				if (DragSlot->Item.Quantity < MIN_REQUIRED_FOR_SLIDER)
				{
					// Is this item located in the character's inventory?
					if (UnderDragSlot->Item.OwningInventory != Interface->GetInventoryComponent()
						&& DragSlot->Item.OwningInventory == Interface->GetInventoryComponent())
					{
						UE_LOG(LogInventoryHUD, Warning, TEXT("Selling the item..."));
						Interface->SellItem(&(DragSlot->Item), 1);
						NPCInventory->SetCurrentTabByItemType(DragSlot->Item.ItemType);
					}
					// Or NPC's?
					else if (UnderDragSlot->Item.OwningInventory == Interface->GetInventoryComponent()
						&& DragSlot->Item.OwningInventory != Interface->GetInventoryComponent())
					{
						UE_LOG(LogInventoryHUD, Warning, TEXT("Buying the item..."));
						Interface->BuyItem(&(DragSlot->Item), 1);
						PlayerInventory->SetCurrentTabByItemType(DragSlot->Item.ItemType);
					}
				}
				else
				{
					CreateTransferWidget(DragSlot);
				}
			}
			
		}
	}
}

