#pragma once


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"


class UInventoryComponent;


UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class ELABORATE_API IInteractableInterface
{
	GENERATED_BODY()


public:
	virtual bool Interact(UInventoryComponent* Inventory) = 0;

};