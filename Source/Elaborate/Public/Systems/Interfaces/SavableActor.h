#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableActor.generated.h"


struct FActorSaveData;

UINTERFACE(MinimalAPI)
class USavableActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for savable actors
 */
class ELABORATE_API ISavableActor
{
	GENERATED_BODY()


public:
	virtual void SaveActorData(FActorSaveData& OutSaveData) const = 0;
	virtual void LoadActorData(const FActorSaveData& InSaveData) = 0;
};