// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/Widgets/ItemInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Systems/Inventory/BaseArmour.h"
#include "Systems/Inventory/BaseWeapon.h"
#include "Systems/Inventory/BaseConsumable.h"
#include "Systems/Inventory/BaseQuestItem.h"
#include "Systems/Inventory/BaseArmour.h"
#include "Systems/Inventory/BaseMiscItem.h"

// Logging
#include "Elaborate.h"

void UItemInfoWidget::NativeConstruct()
{
	if (InspectedItem.ItemClass)
	{
		switch (InspectedItem.ItemType)
		{
		case EItemType::EIT_Armour:
			InitialiseArmourWidget();
			break;
		case EItemType::EIT_Weapon:
			InitialiseWeaponWidget();
			break;
		case EItemType::EIT_Quest:
			InitialiseQuestItemWidget();
			break;
		case EItemType::EIT_Consumable:
			InitialiseConsumableWidget();
			break;
		case EItemType::EIT_Miscellaneous:
			InitialiseMiscItemWidget();
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("InspectedItem Type is None."));
			InitialiseQuestItemWidget();
			break;
		}
	}
}

void UItemInfoWidget::InitCommon(UBaseItem& Item) {
	ItemName->SetText(Item.ItemName);
	ItemDescription->SetText(Item.ItemDescription);

	FString Temp = FString::FromInt((int32)Item.Weight) + "." + FString::FromInt((Item.Weight - (int32)Item.Weight)*10) + " kg";
	ItemWeight->SetText(FText::FromString(Temp));

	Temp = FString::FromInt(Item.Price);
	ItemPrice->SetText(FText::FromString(Temp));
	ItemPrice->SetColorAndOpacity(FSlateColor(FLinearColor(0.8, 0.7, 0.08)));
}

void UItemInfoWidget::InitialiseWeaponWidget()
{
	QuestName->RemoveFromParent();

	UBaseWeapon* DefaultWeapon = Cast<UBaseWeapon>(InspectedItem.ItemClass->GetDefaultObject(true));
	if (DefaultWeapon)
	{
		BaseAttributeName->SetText(FText::FromString("DAMAGE"));
		BaseAttributeName->SetColorAndOpacity(FSlateColor(FLinearColor(0.5f, 0.02f, 0.02f)));

		FString Temp = FString::FromInt(DefaultWeapon->MinBaseDamage) + "-" + FString::FromInt(DefaultWeapon->MaxBaseDamage);
		BaseAttributeValue->SetText(FText::FromString(Temp));

		AddAttributeName1->SetText(FText::FromString("Crit Rate"));
		Temp = "+ " + FString::FromInt(DefaultWeapon->CritRate) + " %";
		AddAttributeValue1->SetText(FText::FromString(Temp));
		AddAttributeValue1->SetColorAndOpacity(FSlateColor(FLinearColor(0.03, 0.3, 0.05)));

		AddAttributeName2->SetText(FText::FromString("Crit DMG"));
		Temp = "+ " + FString::FromInt(DefaultWeapon->CritDamage) + " %";
		AddAttributeValue2->SetText(FText::FromString(Temp));
		AddAttributeValue2->SetColorAndOpacity(FSlateColor(FLinearColor(0.03, 0.3, 0.05)));

		AddAttributeName3->SetText(FText::FromString("Max AOE Range"));
		Temp = FString::FromInt(DefaultWeapon->MaximumAoERange) + " m";
		AddAttributeValue3->SetText(FText::FromString(Temp));
		AddAttributeValue3->SetColorAndOpacity(FSlateColor(FLinearColor(0.03, 0.3, 0.05)));

		if (DefaultWeapon->WeaponPassive != EPassiveWeapon::EPW_None)
		{
			// TODO
			AddAttributeName4->SetText(FText::FromString("SpecialPassive"));
			AddAttributeName4->SetText(FText::FromString("Val"));
		}
		else
		{
			AddAttributeName4->RemoveFromParent();
			AddAttributeValue4->RemoveFromParent();
		}

		InitCommon(*DefaultWeapon);
	}

	InfoBox->ForceLayoutPrepass();
	FVector2D InfoBoxSize = InfoBox->GetDesiredSize();
	UE_LOG(LogInventoryHUD, Warning, TEXT("Desired Box Size: %d"), (int32)InfoBoxSize.Y);

	ClippingBox->SetHeightOverride(InfoBoxSize.Y + 220);
	ClippingBox->SetWidthOverride(420.f);
}

void UItemInfoWidget::InitialiseArmourWidget()
{
	QuestName->RemoveFromParent();
	AddAttributeName2->RemoveFromParent();
	AddAttributeValue2->RemoveFromParent();
	AddAttributeName3->RemoveFromParent();
	AddAttributeValue3->RemoveFromParent();
	AddAttributeName4->RemoveFromParent();
	AddAttributeValue4->RemoveFromParent();

	UBaseArmour* DefaultArmour = Cast<UBaseArmour>(InspectedItem.ItemClass->GetDefaultObject(true));
	if (DefaultArmour)
	{
		BaseAttributeName->SetText(FText::FromString("DAMAGE REDUCTION"));
		BaseAttributeName->SetColorAndOpacity(FSlateColor(FLinearColor(0.5f, 0.02f, 0.02f)));

		FString Temp = FString::FromInt(DefaultArmour->MinDamageReduction * 100) + "-" + 
			FString::FromInt(DefaultArmour->MaxDamageReduction * 100) + " %";
		BaseAttributeValue->SetText(FText::FromString(Temp));

		if (DefaultArmour->ArmourPassive != EPassiveArmour::EPA_None)
		{
			// TODO
// 			AddAttributeName1->SetText(FText::FromString("Crit Rate"));
// 			Temp = "+ " + FString::FromInt(DefaultWeapon->CritRate) + " %";
// 			AddAttributeValue1->SetText(FText::FromString(Temp));
// 			AddAttributeValue1->SetColorAndOpacity(FSlateColor(FLinearColor(0.03, 0.3, 0.05)));
		}
		else
		{
			AddAttributeName1->RemoveFromParent();
			AddAttributeValue1->RemoveFromParent();
		}

		InitCommon(*DefaultArmour);
	}

	SetClippingBoxSize();
}

void UItemInfoWidget::InitialiseQuestItemWidget()
{
	BaseAttributeName->RemoveFromParent();
	BaseAttributeValue->RemoveFromParent();
	AddAttributeName1->RemoveFromParent();
	AddAttributeValue1->RemoveFromParent();
	AddAttributeName2->RemoveFromParent();
	AddAttributeValue2->RemoveFromParent();
	AddAttributeName3->RemoveFromParent();
	AddAttributeValue3->RemoveFromParent();
	AddAttributeName4->RemoveFromParent();
	AddAttributeValue4->RemoveFromParent();
	ItemPrice->RemoveFromParent();
	CoinsIcon->RemoveFromParent();

	UBaseQuestItem* DefaultQuestItem = Cast<UBaseQuestItem>(InspectedItem.ItemClass->GetDefaultObject(true));
	if (DefaultQuestItem)
	{
		InitCommon(*DefaultQuestItem);

		// Set the price back to empty cause the quest items cannot be sold
		ItemPrice->SetText(FText::GetEmpty());

		QuestName->SetText(DefaultQuestItem->QuestName);
	}

	SetClippingBoxSize();
}

void UItemInfoWidget::InitialiseMiscItemWidget()
{
	BaseAttributeName->RemoveFromParent();
	BaseAttributeValue->RemoveFromParent();
	AddAttributeName1->RemoveFromParent();
	AddAttributeValue1->RemoveFromParent();
	AddAttributeName2->RemoveFromParent();
	AddAttributeValue2->RemoveFromParent();
	AddAttributeName3->RemoveFromParent();
	AddAttributeValue3->RemoveFromParent();
	AddAttributeName4->RemoveFromParent();
	AddAttributeValue4->RemoveFromParent();
	QuestName->RemoveFromParent();

	UBaseMiscItem* DefaultMiscItem = Cast<UBaseMiscItem>(InspectedItem.ItemClass->GetDefaultObject(true));
	if (DefaultMiscItem)
	{
		ItemName->SetText(DefaultMiscItem->ItemName);

		FString Temp = FString::FromInt(DefaultMiscItem->Weight) + " kg";
		ItemWeight->SetText(FText::FromString(Temp));

		Temp = FString::FromInt(DefaultMiscItem->Price);
		ItemPrice->SetText(FText::FromString(Temp));
		ItemPrice->SetColorAndOpacity(FSlateColor(FLinearColor(0.8, 0.7, 0.08)));

		ItemDescription->SetText(DefaultMiscItem->ItemDescription);
	}

	SetClippingBoxSize();
}

void UItemInfoWidget::InitialiseConsumableWidget()
{
	AddAttributeName1->RemoveFromParent();
	AddAttributeValue1->RemoveFromParent();
	AddAttributeName2->RemoveFromParent();
	AddAttributeValue2->RemoveFromParent();
	AddAttributeName3->RemoveFromParent();
	AddAttributeValue3->RemoveFromParent();
	AddAttributeName4->RemoveFromParent();
	AddAttributeValue4->RemoveFromParent();
	QuestName->RemoveFromParent();

	UBaseConsumable* DefaultConsumable = Cast<UBaseConsumable>(InspectedItem.ItemClass->GetDefaultObject(true));
	if (DefaultConsumable)
	{
		InitCommon(*DefaultConsumable);
		FString Temp;

		switch (DefaultConsumable->AttributeType)
		{
		case EAttributeType::EAT_Health:
			Temp = "HEALTH ";
			BaseAttributeName->SetColorAndOpacity(FSlateColor(FLinearColor(0.5f, 0.02f, 0.02f)));
			break;
		case EAttributeType::EAT_Mana:
			Temp = "MANA ";
			BaseAttributeName->SetColorAndOpacity(FSlateColor(FLinearColor(0.1f, 0.2f, 0.5f)));
			break;
		case EAttributeType::EAT_Stamina:
			Temp = "STAMINA ";
			BaseAttributeName->SetColorAndOpacity(FSlateColor(FLinearColor(0.1f, 0.5f, 0.1f)));
			break;
		default:
			break;
		}
		switch (DefaultConsumable->EffectType)
		{
		case EConsumableEffect::ECE_InstantRegeneration:
		case EConsumableEffect::ECE_IterativeRegeneration:
			Temp += "REGENERATION";
			break;
		case EConsumableEffect::ECE_RegenerationSpeedup:
			Temp += "REGENERATION SPEEDUP";
			break;
		}
		BaseAttributeName->SetText(FText::FromString(Temp));

		// Decide if its a scaler 
		if (DefaultConsumable->AttributeValue > 1.0f)
		{
			Temp = FString::FromInt(DefaultConsumable->AttributeValue) + " pt";
		}
		else
		{
			Temp = FString::FromInt(DefaultConsumable->AttributeValue * 100) + " %";
		}
		
		BaseAttributeValue->SetText(FText::FromString(Temp));
	}

	SetClippingBoxSize();
}

void UItemInfoWidget::SetClippingBoxSize()
{
	InfoBox->ForceLayoutPrepass();
	FVector2D InfoBoxSize = InfoBox->GetDesiredSize();
	UE_LOG(LogInventoryHUD, Warning, TEXT("Desired Box Size: %d"), (int32)InfoBoxSize.Y);

	ClippingBox->SetHeightOverride(InfoBoxSize.Y + 220);
	ClippingBox->SetWidthOverride(420.f);
}

void UItemInfoWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	RemoveFromParent();
}
