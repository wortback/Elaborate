// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Systems/Inventory/F_InventoryItem.h"

#include "ItemInfoWidget.generated.h"


class UTextBlock;
class USizeBox;
class UVerticalBox;
class UImage;
/**
 * 
 */
UCLASS()
class ELABORATE_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemWeight;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemPrice;

	/** For a weapon, it's damage; for an armour, it's damage reduction; for a consumable it's the affected attribute and the way in which it's affected */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> BaseAttributeName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> BaseAttributeValue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USizeBox> ClippingBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> InfoBox;

	/** */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AddAttributeName1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AddAttributeValue1;

	/** */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AddAttributeName2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AddAttributeValue2;

	/** */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AddAttributeName3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AddAttributeValue3;

	/**  */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AddAttributeName4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AddAttributeValue4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;

	/** Only applicable for quest items */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CoinsIcon;


	
private:
	int32 Width = 420;

	int32 Height = 540;

	F_InventoryItem InspectedItem;

	UTexture2D* ItemPicture = nullptr;

public:
	virtual void NativeConstruct() override;

private:
	void SetClippingBoxSize();

	void InitialiseWeaponWidget();

	void InitialiseArmourWidget();

	void InitialiseQuestItemWidget();

	void InitialiseMiscItemWidget();
	
	void InitialiseConsumableWidget();

	// Initi price, weight, description
	void InitCommon(UBaseItem& Item);

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	void SetInspectedItem(F_InventoryItem Item) { InspectedItem = Item; };

};
