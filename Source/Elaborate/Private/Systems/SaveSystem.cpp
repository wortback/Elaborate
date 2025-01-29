// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/SaveSystem.h"

USaveSystem::USaveSystem() 
{
	SaveSlotNames.Add(TEXT("GlobalSlot"));
	PlayerIndecies.Add(0);

	for (int i = 0; i < SaveSlotNum; ++i)
	{
		SaveSlotNames.Add(FString::Printf(TEXT("LocalSlot%d"), i));
		PlayerIndecies.Add(0);
	}
}
