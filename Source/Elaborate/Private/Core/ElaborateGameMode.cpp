// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ElaborateGameMode.h"
#include "Characters/ElaborateCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include "Systems/SaveSystem.h"
#include "Characters/ElaborateCharacter.h"
#include "Systems/Interfaces/SavableCharacter.h"
#include "Systems/Interfaces/SavableActor.h"


#define GENERATE_JSON 1



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
    UE_LOG(LogTemp, Log, TEXT("Saving game globally..."));
    USaveSystem* SaveGameInstance = Cast<USaveSystem>(UGameplayStatics::CreateSaveGameObject(USaveSystem::StaticClass()));

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        UE_LOG(LogTemp, Log, TEXT("Actor/Character name: %s"), *Actor->GetName());

        if (Actor->GetClass()->ImplementsInterface(USavableActor::StaticClass()))
        {
            UE_LOG(LogTemp, Log, TEXT("Actor implements savable interface."));
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
            UE_LOG(LogTemp, Log, TEXT("Character implements savable interface."));
            FCharacterSaveData CharacterData;
            ISavableCharacter* SavableCharacter = Cast<ISavableCharacter>(Actor);
            if (SavableCharacter)
            {
                SavableCharacter->SaveCharacterData(CharacterData);
                UE_LOG(LogTemp, Warning, TEXT("Saving Location: %s"), *CharacterData.Transform.GetLocation().ToString());
                SaveGameInstance->SavedCharacters.Add(CharacterData);
            }
        }
    }

    FString SaveSlotName = TEXT("GlobalSlot");
    int32 UserIndex = 0;
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex);

#if GENERATE_JSON
    SaveGameAsJson(SaveGameInstance);
#endif
}

void AElaborateGameMode::LoadGameGlobaly()
{
    UE_LOG(LogTemp, Log, TEXT("Loading game globally..."));

    FString SaveSlotName = TEXT("GlobalSlot");
    int32 UserIndex = 0;

    USaveSystem* LoadedGame = Cast<USaveSystem>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
    if (LoadedGame)
    {
        for (const FActorSaveData& ActorData : LoadedGame->SavedActors)
        {
            UE_LOG(LogTemp, Log, TEXT("Loading the actor with name: %s"), *ActorData.ActorName.ToString());

            FString FullActorName = FString::Printf(TEXT("PersistentLevel.%s"), *ActorData.ActorName.ToString());
            AActor* Actor = FindObject<AActor>(GetWorld(), *FullActorName);

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
            UE_LOG(LogTemp, Log, TEXT("Loading the character with name: %s"), *CharacterData.CharacterName.ToString());

            FString FullActorName = FString::Printf(TEXT("PersistentLevel.%s"), *CharacterData.CharacterName.ToString());
            AActor* Actor = FindObject<AActor>(GetWorld(), *FullActorName);

            if (Actor && Actor->GetClass()->ImplementsInterface(USavableCharacter::StaticClass()))
            {
                ISavableCharacter* SavableCharacter = Cast<ISavableCharacter>(Actor);
                if (SavableCharacter)
                {
                    UE_LOG(LogTemp, Log, TEXT("Current Location: %s"), *(Actor->GetTransform().GetLocation()).ToString());
                    UE_LOG(LogTemp, Log, TEXT("Loading Location: %s"), *CharacterData.Transform.GetLocation().ToString());
                    SavableCharacter->LoadCharacterData(CharacterData);
                }
            }
        }
    }
}

FString AElaborateGameMode::ConvertSaveDataToJson(USaveSystem* SaveSystem)
{
    if (!SaveSystem) return TEXT("");

    // Create a JSON object
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    // Serialize SavedActors
    TArray<TSharedPtr<FJsonValue>> SavedActorsJsonArray;
    for (const FActorSaveData& ActorData : SaveSystem->SavedActors)
    {
        TSharedPtr<FJsonObject> ActorJson = MakeShareable(new FJsonObject);
        ActorJson->SetStringField("ActorName", ActorData.ActorName.ToString());
        ActorJson->SetStringField("Transform", ActorData.Transform.ToString());
        SavedActorsJsonArray.Add(MakeShareable(new FJsonValueObject(ActorJson)));
    }
    JsonObject->SetArrayField("SavedActors", SavedActorsJsonArray);

    // Serialize SavedCharacters
    TArray<TSharedPtr<FJsonValue>> SavedCharactersJsonArray;
    for (const FCharacterSaveData& ActorData : SaveSystem->SavedCharacters)
    {
        TSharedPtr<FJsonObject> ActorJson = MakeShareable(new FJsonObject);
        ActorJson->SetStringField("CharacterName", ActorData.CharacterName.ToString());
        ActorJson->SetStringField("Transform", ActorData.Transform.ToString());
        ActorJson->SetStringField("CustomData", ActorData.CustomData);
        SavedCharactersJsonArray.Add(MakeShareable(new FJsonValueObject(ActorJson)));
    }
    JsonObject->SetArrayField("SavedCharacters", SavedCharactersJsonArray);

    // Serialize to a string
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        return OutputString;
    }

    return TEXT("");
}

void AElaborateGameMode::SaveJsonToFile(const FString& JsonString, const FString& FileName)
{
    FString FilePath = FPaths::ProjectSavedDir() + FileName;
    if (FFileHelper::SaveStringToFile(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("JSON file saved to: %s"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save JSON file to: %s"), *FilePath);
    }
}

void AElaborateGameMode::SaveGameAsJson(USaveSystem* SaveGameInstance)
{
    FString JsonString = ConvertSaveDataToJson(SaveGameInstance);
    SaveJsonToFile(JsonString, TEXT("SaveData.json"));
}
