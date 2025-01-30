// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Systems/Inventory/F_InventoryItem.h"

#include "StorageContentSlot.generated.h"

class UButton;
class UTextBlock;
class UItemInfoWidget;
class UStorageContentWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotEnteredEvent, F_InventoryItem, Item);

/**
 * 
 */
UCLASS()
class ELABORATE_API UStorageContentSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> UseButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotText;

	F_InventoryItem* SlotItem;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnSlotEnteredEvent FOnSlotEnteredDelegate;

private:
	FTimerHandle TimerHandle;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

private:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void BroadcastItem();

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

};
