// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Systems/Inventory/F_InventoryItem.h"
#include "InventorySlot.generated.h"


class UPrimaryHUDWidget;
class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ELABORATE_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> UseButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmountText;

	/** Slot background an empty slot (default) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slot Texture")
	UTexture2D* EmptyBG = nullptr;
	
	/** Slot background for item of type common */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slot Texture")
	UTexture2D* CommonBG = nullptr;

	/** Slot background for item of type common */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slot Texture")
	UTexture2D* UncommonBG = nullptr;

	/** Slot background for item of type common */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slot Texture")
	UTexture2D* RareBG = nullptr;

	/** Slot background for item of type common */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slot Texture")
	UTexture2D* DivineBG = nullptr;

	/** Slot background for item of type common */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Slot Texture")
	UTexture2D* CursedBG = nullptr;

public:
	F_InventoryItem Item;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(ExposeOnSpawn = "true"))
	UPrimaryHUDWidget* OwningHUD;

	FTimerHandle TimerHandle;
public:
	void UpdateSlotContent();

	virtual void NativeConstruct() override;

private:
	/************************************************************************/
	/* Native Events                                                                     */
	/************************************************************************/
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	//////////////////////////////////////////////////////////////////////////

	void SetBackgroundByItemRarity(EItemRarity ItemRarity);
};
