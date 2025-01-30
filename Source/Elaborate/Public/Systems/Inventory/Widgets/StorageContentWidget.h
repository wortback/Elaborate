// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StorageContentWidget.generated.h"


class UWrapBox;
class ABaseStorage;
struct FInventoryItem;
class UStorageContentSlot;
class UItemInfoWidget;

/**
 * 
 */
UCLASS()
class ELABORATE_API UStorageContentWidget : public UUserWidget
{
	GENERATED_BODY()
	
	friend class UStorageContentSlot;

public:
	/** WrapBox that contains slots that represent the items stored in the storage that spawns the widget */
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWrapBox> SlotBox;

	/** Storage Actor that spawned this widget */
	UPROPERTY(EditAnywhere, Category = "Subwidget classes")
	TSubclassOf<ABaseStorage> OwningStorageClass;

	ABaseStorage* OwningStorage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Subwidget classes")
	TSubclassOf<UItemInfoWidget> InfoWidgetClass;

	UItemInfoWidget* InfoWidget = nullptr;

	

public:
	virtual void NativeConstruct() override;

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void CreateInfoWidget(FInventoryItem Item);
};
