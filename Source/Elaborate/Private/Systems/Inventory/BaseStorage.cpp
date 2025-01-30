// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Inventory/BaseStorage.h"
#include "Systems/Inventory/FInventoryItem.h"
#include "Components/StaticMeshComponent.h"
#include "Systems/Inventory/Widgets/StorageContentWidget.h"
#include "Systems/Inventory/Widgets/StorageContentSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Inventory/BaseConsumable.h"
#include "Components/WrapBox.h"

// Logging
#include "Elaborate.h"


// Sets default values
ABaseStorage::ABaseStorage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));

	SetRootComponent(DefaultSceneComponent);
	DisplayMesh->SetupAttachment(RootComponent);

	IntialiseStorage();
}

// Called when the game starts or when spawned
void ABaseStorage::BeginPlay()
{
	Super::BeginPlay();

	RuntimeItems = OriginalItems;
}

void ABaseStorage::IntialiseStorage()
{
	OriginalItems.SetNum(Capacity);
	for (int i = 0; i <= Capacity - 1; ++i)
	{
		OriginalItems[i].Quantity = 0;
		OriginalItems[i].IndexLocation = i;
		OriginalItems[i].ItemType = EItemType::EIT_None;
		OriginalItems[i].ItemClass = UBaseItem::StaticClass();
	}	
}

void ABaseStorage::GenerateRandomItems()
{
	// TODO
}

void ABaseStorage::DisplayContent()
{
	for (int i = 0; i <= Capacity -1; ++i)
	{
		UE_LOG(LogInventoryHUD, Log, TEXT("%d: type %s"), i, *EItemTypeToString(RuntimeItems[i].ItemType));
		UE_LOG(LogInventoryHUD, Log, TEXT("%d: quantity %d"), i, RuntimeItems[i].Quantity);
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ContentWidget = CreateWidget<UStorageContentWidget>(PlayerController, ContentWidgetClass);
		if (ContentWidget)
		{
			UE_LOG(LogInventoryHUD, Log, TEXT("Contents of the interacting storage actor are displayed."));
			ContentWidget->OwningStorage = this;

			for (FInventoryItem& Item : RuntimeItems)
			{
				if (Item.ItemType != EItemType::EIT_None)
				{
					UStorageContentSlot* ChildSlot = CreateWidget<UStorageContentSlot>(GetWorld(), ContentSlotClass);
					ChildSlot->SlotItem = &Item;
					ChildSlot->FOnSlotEnteredDelegate.AddDynamic(ContentWidget, &UStorageContentWidget::CreateInfoWidget);
					ContentWidget->SlotBox->AddChildToWrapBox(ChildSlot);
				}
			}
			
			ContentWidget->AddToViewport();
			ContentWidget->SetPositionInViewport(FVector2D(300, 300));

			PlayerController->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetWidgetToFocus(ContentWidget->TakeWidget());
			PlayerController->SetInputMode(InputMode);
		}
	}
}

void ABaseStorage::HideContent()
{
	if (ContentWidget)
	{
		ContentWidget->RemoveFromParent();
		ContentWidget = nullptr;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

bool ABaseStorage::Interact(UInventoryComponent* Inventory)
{
	UE_LOG(LogInventoryHUD, Log, TEXT("Intarect for BaseStorage is called."));

	// If there's at least one item in the inventory that is not of type NONE, display the storage content
	// TODO: Maybe optimise to prevent two times iterating over the array (one time here, one in DisplayContent)
	for (FInventoryItem& Item : RuntimeItems)
	{
		if (ContentSlotClass)
		{
			if (Item.ItemType != EItemType::EIT_None)
			{
				DisplayContent();
				return true;
			}
		}
	}
	return false;
}

// Called every frame
void ABaseStorage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

