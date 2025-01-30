// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Systems/Inventory/CommonTypes.h"
#include "Systems/Inventory/F_InventoryItem.h"
#include "Systems/Interfaces/StorageInterface.h"
#include "Systems/Interfaces/InteractableInterface.h"

#include "BaseStorage.generated.h"


class UStaticMeshComponent;
class UInventoryComponent;
class UStorageContentWidget;
class UStorageContentSlot;

UENUM(Blueprintable)
enum class EStorageRank : uint8
{
	ESR_Common,
	ESR_Exquisite,
	ESR_Luxurious,
};

/**
 * Base Class for all containers that are capable of storing items (e.g. chests, strongboxes, barrels, etc)
 * 
 */
UCLASS()
class ELABORATE_API ABaseStorage : public AActor, public IStorageInterface, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseStorage();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Collision")
	UStaticMeshComponent* DisplayMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UStorageContentWidget> ContentWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UStorageContentSlot> ContentSlotClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	UStorageContentWidget* ContentWidget = nullptr;

protected:
	/** Separate array for runtime items management. Items will be added/removed here during the game */
	UPROPERTY(Transient, meta = (AllowPrivateAccess = "true"))
	TArray<F_InventoryItem> RuntimeItems;

	/** Contains manually set items */
	UPROPERTY(EditAnywhere, Category = "Storage Defaults", meta = (AllowPrivateAccess = "true"))
	TArray<F_InventoryItem> OriginalItems;

	/** Storage capacity */
	UPROPERTY(EditAnywhere, Category = "Storage Defaults", meta = (AllowPrivateAccess = "true"))
	int32 Capacity = 10;

	/** The maximum number of items that can be generated */
	UPROPERTY(EditAnywhere, Category = "Item Generation", meta = (AllowPrivateAccess = "true"))
	int32 GenerationRange = 10;

	/** List of item classes which can be generated */
	UPROPERTY(EditAnywhere, Category = "Item Generation", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UBaseItem>> GeneratableClasses;

	/** Determines how rare/valuable the items stored will be */
	UPROPERTY(EditAnywhere, Category = "Storage Defaults", meta = (AllowPrivateAccess = "true"))
	EStorageRank StorageRank = EStorageRank::ESR_Common;

	/** True if stored items should not randomly respawn after a period of time */
	UPROPERTY(EditAnywhere, Category = "Storage Defaults", meta = (AllowPrivateAccess = "true"))
	bool bIsPersistent = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region StorageInterface
public:
	/** Initialises items stored in the storage */
	virtual void IntialiseStorage() override;

	/** Generates random items in the provided array */
	virtual void GenerateRandomItems() override;

	/**
	 * @brief Displays stored items on the screen.
	 * 
	 * This function creates a ContentWidget that displays the content
	 * in vertically organised slots.
	 */
	virtual void DisplayContent() override;

	/**
	 * @brief Removes stored items from the screen.
	 *
	 * This function destroys the ContentWidget.
	 */
	virtual void HideContent() override;

	/**
	 * @return true if the storage is not empty
	 * 
	 * That is if the Items array has at least one item of the type that it not EIT_None
	 */
	virtual bool Interact(UInventoryComponent* Inventory) override;

#pragma endregion StorageInterface

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE const TArray<F_InventoryItem>& GetStoredItems() const { return RuntimeItems; };
	FORCEINLINE TArray<F_InventoryItem>& SetStoredItems() { return RuntimeItems; };

};
