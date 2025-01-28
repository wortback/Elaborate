// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ElaborateGameMode.h"
#include "Characters/ElaborateCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"

#include "Systems/SaveSystem.h"
#include "Characters/ElaborateCharacter.h"
#include "Systems/Interfaces/SavableCharacter.h"
#include "Systems/Interfaces/SavableActor.h"

AElaborateGameMode::AElaborateGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AElaborateGameMode::SaveGameGlobaly()
{
    USaveSystem* SaveGameInstance = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveSystem::StaticClass()));

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;

        if (Actor->GetClass()->ImplementsInterface(USavableActor::StaticClass()))
        {
            FActorSaveData ActorData;
            ISavableActor* SavableActor = Cast<ISavableActor>(Actor);
            if (SavableActor)
            {
                SavableActor->SaveActorData(ActorData);
                SaveGameInstance->SavedActors.Add(ActorData);
            }
        }
        if (Actor->GetClass()->ImplementsInterface(USavableCharacter::StaticClass()))
        {
            FCharacterSaveData CharacterData;
            ISavableCharacter* SavableCharacter = Cast<ISavableCharacter>(Actor);
            if (SavableCharacter)
            {
                SavableCharacter->SaveCharacterData(CharacterData);
                SaveGameInstance->SavedCharacters.Add(CharacterData);
            }
        }
    }

    FString SaveSlotName = TEXT("GlobalSlot");
    int32 UserIndex = 0;
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex);
}

void AElaborateGameMode::LoadGameGlobaly()
{
    FString SaveSlotName = TEXT("GlobalSlot");
    int32 UserIndex = 0;

    USaveSystem* LoadedGame = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
    if (LoadedGame)
    {
        for (const FActorSaveData& ActorData : LoadedGame->SavedActors)
        {
            AActor* Actor = FindObject<AActor>(GetWorld(), *ActorData.ActorName.ToString());
            if (Actor && Actor->GetClass()->ImplementsInterface(USavableActor::StaticClass()))
            {
                ISavableActor* SavableActor = Cast<ISavableActor>(Actor);
                if (SavableActor)
                {
                    SavableActor->LoadActorData(ActorData);
                }
            }
        }
        for (const FCharacterSaveData& CharacterData : LoadedGame->SavedCharacters)
        {
            AActor* Actor = FindObject<AActor>(GetWorld(), *CharacterData.CharacterName.ToString());
            if (Actor && Actor->GetClass()->ImplementsInterface(USavableCharacter::StaticClass()))
            {
                ISavableCharacter* SavableCharacter = Cast<ISavableCharacter>(Actor);
                if (SavableCharacter)
                {
                    SavableCharacter->LoadCharacterData(CharacterData);
                }
            }
        }
    }
}
