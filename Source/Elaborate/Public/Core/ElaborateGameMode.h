// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ElaborateGameMode.generated.h"

UCLASS(minimalapi)
class AElaborateGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AElaborateGameMode();

	UFUNCTION()
	void SaveGameGlobaly();

	UFUNCTION()
	void LoadGameGlobaly();
};



