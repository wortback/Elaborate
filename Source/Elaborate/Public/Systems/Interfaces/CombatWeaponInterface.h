#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatWeaponInterface.generated.h"


struct FCharacterSaveData;

UINTERFACE(MinimalAPI)
class UCombatWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for savable actors
 */
class ELABORATE_API ICombatWeaponInterface
{
	GENERATED_BODY()


public:
	virtual void Attack() = 0;

	virtual void HeavyAttack() = 0;
};