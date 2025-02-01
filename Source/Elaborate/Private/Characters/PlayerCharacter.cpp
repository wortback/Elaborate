// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Systems/Inventory/WorldItem.h"
#include "Systems/Inventory/Widgets/PrimaryHUDWidget.h"
#include "Systems/Inventory/InventoryComponent.h"
#include "DrawDebugHelpers.h"
#include "Systems/Inventory/BaseStorage.h"
#include "Systems/Inventory/FInventoryItem.h"
#include "Systems/Inventory/CommonTypes.h"

#include "Systems/SaveSystem.h"
#include "Core/ElaborateGameMode.h"

#include "Elaborate.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter);

//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	PlayerInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventoryComponent"));
}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Create primary HUD
	if (PrimaryHUDWidgetClass)
	{
		// Create the widget
		PrimaryHUDWidget = CreateWidget<UPrimaryHUDWidget>(GetWorld(), PrimaryHUDWidgetClass);

		// Check if widget was created successfully
		if (PrimaryHUDWidget)
		{
			// Add it to the viewport
			PrimaryHUDWidget->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Warning, TEXT("PrimaryHUDWidgetClass is not set"));
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	// TODO Optimise LookAt call by calling it once a second instead of every frame
	ViewedActor = LookAt();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Yellow,
			FString::Printf(TEXT("Equipped Armour Type %s"), *EItemTypeToString(PlayerInventoryComponent->GetEquippedArmour().ItemType)));
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);

		// Open Inventory Menu
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OpenInventory);
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
	//UE_LOG(LogPlayerCharacter, Log, TEXT("Interact action is called."));

	IInteractableInterface* Interface = Cast<IInteractableInterface>(ViewedActor);
	if (Interface)
	{
		Interface->Interact(PlayerInventoryComponent);
	}
}

void APlayerCharacter::OpenInventory(const FInputActionValue& Value)
{
	UE_LOG(LogPlayerCharacter, Log, TEXT("Inventory opened"));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		if (PrimaryHUDWidget)
		{
			InputMode.SetWidgetToFocus(PrimaryHUDWidget->TakeWidget());
		}

		PlayerController->SetInputMode(InputMode);
	}

	if (PrimaryHUDWidget)
	{
		PrimaryHUDWidget->SetFocus();
	}

	OpenPlayerInventory();
}

#pragma region InventoryHUDInterfaceImplementation
AActor* APlayerCharacter::LookAt()
{
	// Trace a BaseItem
	FHitResult ItemTraceResult;

	FRotator CameraRotation = FollowCamera->GetComponentRotation();
	FVector Start = GetMesh()->GetSocketLocation("head") + (CameraRotation.Vector() * 10.0f);

	FVector End = (CameraRotation.Vector() * 150.0f) + Start;
	FCollisionQueryParams CollisionQueryParamsLower;
	CollisionQueryParamsLower.AddIgnoredActor(this);
	CollisionQueryParamsLower.AddIgnoredComponent(GetMesh());
	GetWorld()->LineTraceSingleByChannel(ItemTraceResult, Start, End, ECollisionChannel::ECC_Interactable);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 3.0f); // Line thickness is 5.0f

	if (ItemTraceResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), ItemTraceResult.Location, 5.f, 12, FColor::Red, false, 5.f);

		return ItemTraceResult.GetActor();
	}

	return nullptr;
}

void APlayerCharacter::OpenPlayerInventory()
{
	UE_LOG(LogPlayerCharacter, Log, TEXT("OpenPlayerInventory is called."));


	UpdateInventoryHUD(PlayerInventoryComponent);

	if (PrimaryHUDWidget)
	{
		PrimaryHUDWidget->ShowPlayerInventory(true);
	}
}

void APlayerCharacter::OpenNPCInventory(UInventoryComponent* NPCInventoryComponent)
{
	UE_LOG(LogPlayerCharacter, Log, TEXT("OpenNPCInventory is called."));

	UpdateNPCComponentPtr(NPCInventoryComponent);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		if (PrimaryHUDWidget)
		{
			InputMode.SetWidgetToFocus(PrimaryHUDWidget->TakeWidget());

			PlayerController->SetInputMode(InputMode);
			PrimaryHUDWidget->SetFocus();

			UpdateInventoryHUD(PlayerInventoryComponent, NPCInventoryComponent);

			PrimaryHUDWidget->ShowNPCInventory(true);
		}
	}
}

void APlayerCharacter::UpdateInventoryHUD(UInventoryComponent* PlayerComp, UInventoryComponent* NPCComp)
{
	UE_LOG(LogPlayerCharacter, Log, TEXT("UpdateInventoryHUD is called."));
	if (PrimaryHUDWidget)
	{
		PrimaryHUDWidget->UpdateInventory(PlayerComp, NPCComp);
	}
}

void APlayerCharacter::UpdateInventoryHUD(UInventoryComponent* InventoryComponent)
{
	UE_LOG(LogPlayerCharacter, Log, TEXT("UpdateInventoryHUD is called."));
	if (PrimaryHUDWidget)
	{
		PrimaryHUDWidget->UpdateInventory(InventoryComponent);
	}
}

void APlayerCharacter::UpdateInventoryHUD()
{
	UE_LOG(LogPlayerCharacter, Log, TEXT("UpdateInventoryHUD is called."));

	if (PrimaryHUDWidget)
	{
		if (NPCInventoryComp)
		{
			PrimaryHUDWidget->UpdateInventory(PlayerInventoryComponent, NPCInventoryComp);
		}
		else
		{
			PrimaryHUDWidget->UpdateInventory(PlayerInventoryComponent);
		}
	}

}

void APlayerCharacter::RemoveItem(FInventoryItem* Item, int32 Quantity)
{
	UE_LOG(LogPlayerCharacter, Log, TEXT("AMainCharacter::RemoveItem is called"));
	PlayerInventoryComponent->RemoveItem(Item, Quantity);
}

bool APlayerCharacter::ProcessItem(FInventoryItem* Item, int32 Quantity)
{
	return PlayerInventoryComponent->ProcessItem(Item, Quantity, true);
}

bool APlayerCharacter::EquipItem(FInventoryItem* Item, int32 Location)
{
	// If the character is in the trade mode with an NPC, don't equip items. 
	if (!NPCInventoryComp)
	{
		return PlayerInventoryComponent->EquipItem(Item, Location);
	}
	return false;
}

bool APlayerCharacter::UnequipItem(FInventoryItem* Item, int32 Location)
{
	if (!NPCInventoryComp)
	{
		return PlayerInventoryComponent->UnequipItem(Item, Location);
	}
	return false;
}

bool APlayerCharacter::SellItem(FInventoryItem* Item, int32 Quantity)
{
	// If the character is in the trade mode with an NPC, don't equip items. 
	if (NPCInventoryComp)
	{
		if (NPCInventoryComp->ProcessItem(Item, Quantity, true))
		{
			if (PlayerInventoryComponent->RemoveItem(Item, Quantity))
			{
				UE_LOG(LogPlayerCharacter, Log, TEXT("Item removed successfully."));
			}

			return true;
		}
	}
	return false;
}

bool APlayerCharacter::BuyItem(FInventoryItem* Item, int32 Quantity)
{
	// If the character is in the trade mode with an NPC, don't equip items. 
	if (NPCInventoryComp)
	{
		if (NPCInventoryComp->RemoveItem(Item, Quantity))
		{
			PlayerInventoryComponent->ProcessItem(Item, Quantity, true);
			return true;
		}
	}
	return false;
}

void APlayerCharacter::UpdateNPCComponentPtr(UInventoryComponent* InventoryComp)
{
	if (InventoryComp)
	{
		bIsTrading = true;
	}
	else
	{
		bIsTrading = false;
	}
	NPCInventoryComp = InventoryComp;
}

#pragma endregion InventoryHUDInterfaceImplementation

# pragma region SavableCharacterInterface
void APlayerCharacter::SaveGameGlobally()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

	if (GameMode)
	{
		AElaborateGameMode* GM = Cast<AElaborateGameMode>(GameMode);
		if (GM)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current GameMode: %s"), *GameMode->GetName());
			GM->SaveGameGlobaly();
		}
	}
}

void APlayerCharacter::LoadGameGlobally()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

	if (GameMode)
	{
		AElaborateGameMode* GM = Cast<AElaborateGameMode>(GameMode);
		if (GM)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current GameMode: %s"), *GameMode->GetName());
			GM->LoadGameGlobaly();
		}
	}
}

void APlayerCharacter::SaveGameLocally(int32 SlotIndex)
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

	if (GameMode)
	{
		AElaborateGameMode* GM = Cast<AElaborateGameMode>(GameMode);
		if (GM)
		{
			UE_LOG(LogTemp, Log, TEXT("Current GameMode: %s"), *GameMode->GetName());
			GM->SaveGameLocally(SlotIndex);
		}
	}
}

void APlayerCharacter::LoadGameLocally(int32 SlotIndex)
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

	if (GameMode)
	{
		AElaborateGameMode* GM = Cast<AElaborateGameMode>(GameMode);
		if (GM)
		{
			UE_LOG(LogTemp, Log, TEXT("Current GameMode: %s"), *GameMode->GetName());
			GM->LoadGameLocally(SlotIndex);
		}
	}
}

void APlayerCharacter::SaveCharacterData(FCharacterSaveData& OutSaveData) const
{
	OutSaveData.CharacterName = GetFName();
	OutSaveData.Transform = GetActorTransform();

	// Serialize custom data as a string (e.g., JSON)
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutSaveData.CustomData);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("Level"), Level);
	Writer->WriteValue(TEXT("CurrentQuestID"), QuestID);
	Writer->WriteObjectEnd();
	Writer->Close();
}

void APlayerCharacter::LoadCharacterData(const FCharacterSaveData& InSaveData)
{
	SetActorTransform(InSaveData.Transform);

	// Deserialize custom data from string
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InSaveData.CustomData);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		Level = JsonObject->GetNumberField(TEXT("Level"));
		QuestID = JsonObject->GetStringField(TEXT("CurrentQuestID"));
	}
}

# pragma endregion SavableCharacterInterface