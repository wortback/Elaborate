// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSystem.generated.h"



/**
 *  Struct for storing save data of a character
 * 
 */
USTRUCT(BlueprintType)
struct FSystemSaveData
{
    GENERATED_BODY()


    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FRotator ControllerRotation;
};

/**
 *  Struct for storing save data of an actor
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
 *  Struct for storing save data of a character
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

    
    UPROPERTY(BlueprintReadWrite, Category = "SaveData | Saved")
    TArray<FCharacterSaveData> SavedCharacters;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData | Saved")
    TArray<FActorSaveData> SavedActors;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData | Saved")
    FSystemSaveData SavedSystemData;

public:
    USaveSystem();

};
