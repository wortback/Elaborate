#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatCharacterInterface.generated.h"


class AWorldItem;

UINTERFACE(MinimalAPI)
class UCombatCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for savable actors
 */
class ELABORATE_API ICombatCharacterInterface
{
	GENERATED_BODY()


public:
	virtual void EquipWeapon(AWorldItem* EquippedWeapon) = 0;
};