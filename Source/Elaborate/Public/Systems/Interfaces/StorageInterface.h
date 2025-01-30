#pragma once


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StorageInterface.generated.h"




UINTERFACE(MinimalAPI)
class UStorageInterface : public UInterface
{
	GENERATED_BODY()
};

class ELABORATE_API IStorageInterface
{
	GENERATED_BODY()


public:
	virtual void DisplayContent() = 0;

	virtual void HideContent() = 0;

private:
	virtual void IntialiseStorage() = 0;

	virtual void GenerateRandomItems() = 0;
};