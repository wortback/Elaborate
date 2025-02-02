// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElaborateGameMode.h"
#include "ElaborateCharacter.h"
#include "UObject/ConstructorHelpers.h"

AElaborateGameMode::AElaborateGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
