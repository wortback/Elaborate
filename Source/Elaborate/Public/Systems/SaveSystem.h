// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSystem.generated.h"


/**
 *  Struct that holds actor's data that has to persist 
 * 
 */
USTRUCT(BlueprintType)
struct FActorSaveData
{
    GENERATED_BODY()


    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FName ActorName;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FTransform Transform;
};

/**
 *  Struct that holds actor's data that has to persist 
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterSaveData
{
    GENERATED_BODY()

 
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FName CharacterName;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FTransform Transform;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FString CustomData;
};

/**
 * 
 */
UCLASS()
class ELABORATE_API USaveSystem : public USaveGame
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    int32 SaveSlotNum = 5;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TArray<FString> SaveSlotNames;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TArray<int32> PlayerIndecies;


    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TArray<FCharacterSaveData> SavedCharacters;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TArray<FActorSaveData> SavedActors;

public:
    USaveSystem();

};
