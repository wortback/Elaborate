// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ElaborateGameMode.generated.h"

class USaveSystem;

UCLASS(minimalapi)
class AElaborateGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AElaborateGameMode();

	UFUNCTION(BlueprintCallable)
	void SaveGameGlobaly();

	UFUNCTION(BlueprintCallable)
	void LoadGameGlobaly();

private:
	FString ConvertSaveDataToJson(USaveSystem* SaveSystem);

	void SaveJsonToFile(const FString& JsonString, const FString& FileName);

	void SaveGameAsJson(USaveSystem* SaveSystem);
};



