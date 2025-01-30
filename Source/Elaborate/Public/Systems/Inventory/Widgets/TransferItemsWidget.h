// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TransferItemsWidget.generated.h"

class UTextBlock;
class UBaseSlider;
class UInventorySlot;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSliderValueConfirmedEvent, int32, Quantity);



UCLASS()
class ELABORATE_API UTransferItemsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Question;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SliderValue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBaseSlider> Slider;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnSliderValueConfirmedEvent FOnSliderValueConfirmedDelegate;

private:
	/** Upper limit for the slider value */
	int32 MaxValue;

	/** The slot with the item this widget was originally created for */
	UInventorySlot* ItemSlot = nullptr;

	void ConfirmSliderValue();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void UpdateSliderValue(float Value);

public:
	virtual void NativeConstruct() override;

	FORCEINLINE void SetMaxValue(int32 NewMaxValue) { MaxValue = NewMaxValue; }
	FORCEINLINE void SetItemSlot(UInventorySlot* NewSlot) { ItemSlot = NewSlot; }
	FORCEINLINE UInventorySlot* GetItemSlot() const { return ItemSlot; }

};
