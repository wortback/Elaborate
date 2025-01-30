#pragma once


#include "CoreMinimal.h"
#include "CommonTypes.generated.h"

//////////////////////////////////////////////////////////////////////////
///		ENUMS
//////////////////////////////////////////////////////////////////////////

UENUM()
enum class EItemType : uint8
{
	EIT_Armour,
    EIT_Weapon,
	EIT_Quest,
	EIT_Consumable,
    EIT_Miscellaneous,
    EIT_None
};

inline FString EItemTypeToString(EItemType ItemType)
{
	switch (ItemType)
	{
	case EItemType::EIT_Armour:
		return FString("Armour");
		break;
	case EItemType::EIT_Weapon:
		return FString("Weapon");
		break;
	case EItemType::EIT_Quest:
		return FString("Quest");
		break;
	case EItemType::EIT_Consumable:
		return FString("Consumable");
		break;
	case EItemType::EIT_Miscellaneous:
		return FString("Misc");
		break;
	case EItemType::EIT_None:
	default:
		return FString("None");
		break;
	}
}

UENUM()
enum class EInventoryWidgetTab : uint8
{
	EIWT_Equipment,
	EIWT_Quest,
	EIWT_Consumable,
    EIWT_Miscellaneous,
};

/**
 * NB: Items of class Divine can only be equipped by the Cleric Class
 *	   Items of class Cursed can only be equipped by the Mage Class
 */
UENUM()
enum class EItemRarity : uint8
{
	EIR_Common,
	EIR_Uncommon,
	EIR_Rare,
	EIR_Divine,
	EIR_Cursed,
};

inline FString EItemRarityToString(EItemRarity ItemRarity)
{
	switch (ItemRarity)
	{
	case EItemRarity::EIR_Uncommon:
		return FString("EIR_Uncommon");
		break;
	case EItemRarity::EIR_Rare:
		return FString("EIR_Rare");
		break;
	case EItemRarity::EIR_Divine:
		return FString("EIR_Divine");
		break;
	case EItemRarity::EIR_Cursed:
		return FString("EIR_Cursed");
		break;
	case EItemRarity::EIR_Common:
	default:
		return FString("EIR_Common");
		break;
	}
}

UENUM()
enum class EAttributeType : uint8
{
	EAT_Health,
	EAT_Stamina,
	EAT_Mana,
	EAT_None,
};

/**
 * NB: Special Effect is thought for any effect that doesn't fall into two other categories
 */
UENUM()
enum class EConsumableEffect : uint8
{
	ECE_InstantRegeneration,
	ECE_IterativeRegeneration,
	ECE_RegenerationSpeedup,
	ECE_SpecialEffect,
};