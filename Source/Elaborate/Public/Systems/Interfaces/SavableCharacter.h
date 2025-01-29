#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableCharacter.generated.h"


struct FCharacterSaveData;

UINTERFACE(MinimalAPI)
class USavableCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for savable actors
 */
class ELABORATE_API ISavableCharacter
{
	GENERATED_BODY()


public:
	virtual void SaveCharacterData(FCharacterSaveData& OutSaveData) const = 0;
	virtual void LoadCharacterData(const FCharacterSaveData& InSaveData) = 0;
};