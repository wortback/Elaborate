// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Characters/BaseCharacter.h"

#include "Systems/Interfaces/SavableCharacter.h"
#include "Systems/Interfaces/CombatCharacterInterface.h"
#include "Systems/Interfaces/InventoryHUDInterface.h"

#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInventoryComponent;
class UPrimaryHUDWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);


UCLASS(config=Game)
class APlayerCharacter : public ABaseCharacter, public IInventoryHUDInterface, public ISavableCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Component that handles player's inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* PlayerInventoryComponent;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PrimaryHUDWidgetClass;

	/* Primary HUD on top of which any other HUD such as Inventory etc is mounted */
	UPrimaryHUDWidget* PrimaryHUDWidget;

private:
	/** Actor the character is currently looking at */
	AActor* ViewedActor = nullptr;

	UInventoryComponent* NPCInventoryComp = nullptr;

	/** True, if the character is exchanging items with an NPC (NPCInventoryComp is not nullptr) */
	bool bIsTrading;

private:
	int32 Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = "true"))
	FString QuestID = TEXT("IT0Q0");

public:
	APlayerCharacter();

	UFUNCTION(BlueprintCallable)
	void SaveGameGlobally();

	UFUNCTION(BlueprintCallable)
	void LoadGameGlobally();

	UFUNCTION(BlueprintCallable)
	void SaveGameLocally(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void LoadGameLocally(int32 SlotIndex);
	
public:
	virtual void SaveCharacterData(FCharacterSaveData& OutSaveData) const override;

	virtual void LoadCharacterData(const FCharacterSaveData& InSaveData) override;
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interacting input */
	void Interact(const FInputActionValue& Value);

	/** Called to open the inventory belonging to the character */
	void OpenInventory(const FInputActionValue& Value);


#pragma region InventoryHUDInterface
protected:
	virtual AActor* LookAt() override;

	virtual void OpenPlayerInventory() override;

	virtual void OpenNPCInventory(UInventoryComponent* NPCInventoryComponent) override;

	/** 
	* Depending on whether the character is trading with an NPC, updates PlayerInventoryWindow and NPCInventoryWindow
	* or the inventory menu */
	virtual void UpdateInventoryHUD() override;

	virtual bool GetIsInTradeMode() const { return bIsTrading; }

	virtual void SetIsInTradeMode(bool bIsInTradeMode) { bIsTrading = bIsInTradeMode; }

	virtual void RemoveItem(FInventoryItem* Item, int32 Quantity) override;

	virtual bool ProcessItem(FInventoryItem* Item, int32 Quantity) override;

	virtual bool EquipItem(FInventoryItem* Item, int32 Location = -1) override;

	virtual bool UnequipItem(FInventoryItem* Item, int32 Location = -1) override;

	virtual bool SellItem(FInventoryItem* Item, int32 Quantity) override;

	virtual bool BuyItem(FInventoryItem* Item, int32 Quantity) override;

	virtual void UpdateNPCComponentPtr(UInventoryComponent* InventoryComp) override;

	virtual UInventoryComponent* GetInventoryComponent() const override { return PlayerInventoryComponent; }

private:
	/** Updates inventory menu */
	void UpdateInventoryHUD(UInventoryComponent* InventoryComponent);

	/** Updates PlayerInventoryWindow and NPCInventoryWindow */
	void UpdateInventoryHUD(UInventoryComponent* PlayerComp, UInventoryComponent* NPCComp);

#pragma endregion InventoryHUDInterface
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

