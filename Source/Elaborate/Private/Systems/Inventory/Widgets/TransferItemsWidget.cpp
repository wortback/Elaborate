// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/TransferItemsWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Inventory/Widgets/WidgetComponents/BaseSlider.h"



void UTransferItemsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Slider->OnValueChanged.AddUniqueDynamic(this, &UTransferItemsWidget::UpdateSliderValue);

	Question->SetText(FText::FromString("HOW MANY"));
	SliderValue->SetText(FText::FromString(FString::FromInt(MaxValue)));

	//Slider setup
	Slider->SetMinValue(1);
	Slider->SetMaxValue(MaxValue);
	Slider->SetStepSize(1);
	Slider->SetValue(MaxValue);

	SetIsFocusable(true);
}

void UTransferItemsWidget::UpdateSliderValue(float NewValue)
{
	//UE_LOG(LogTemp, Warning, TEXT("Slider value is updated %f"), NewValue);
	SliderValue->SetText(FText::FromString(FString::FromInt((int32)NewValue)));
}

void UTransferItemsWidget::ConfirmSliderValue()
{
	UE_LOG(LogTemp, Log, TEXT("Slider value is %d"), (int32)Slider->GetValue());
	FOnSliderValueConfirmedDelegate.Broadcast((int32)Slider->GetValue());
	RemoveFromParent();
}

FReply UTransferItemsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FName Key = InKeyEvent.GetKey().GetFName();
	if (Key == "Enter" || Key == "E")
	{
		ConfirmSliderValue();
	}
	else if (Key == "Tab" || Key == "Escape")
	{
		RemoveFromParent();
	}
	return FReply::Handled();
}


