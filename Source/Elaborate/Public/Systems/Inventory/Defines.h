#pragma once


#include "CoreMinimal.h"


//////////////////////////////////////////////////////////////////////////
///		InventoryComponent 
//////////////////////////////////////////////////////////////////////////


constexpr int32 EQ_ARMOUR_INDEX_LOCATION	= -2;
constexpr int32 EQ_WEAPON_INDEX_LOCATION	= -3;
constexpr int32 QUICK_ITEM_1_INDEX_LOCATION = -4;
constexpr int32 QUICK_ITEM_2_INDEX_LOCATION = -5;
constexpr int32 QUICK_ITEM_3_INDEX_LOCATION = -6;
constexpr int32 QUICK_ITEM_4_INDEX_LOCATION = -7;

// Capacity extension factor (the number of slots added)
constexpr int32 CAPACITY_INCREASE_FACTOR	= 15;


/** True if the index matches either one of the indices of equipped items or QA items */
inline bool IsSpecialItemIndex(int32 Index)
{
	switch (Index)
	{
	case EQ_ARMOUR_INDEX_LOCATION:
	case EQ_WEAPON_INDEX_LOCATION:
	case QUICK_ITEM_1_INDEX_LOCATION:
	case QUICK_ITEM_2_INDEX_LOCATION:
	case QUICK_ITEM_3_INDEX_LOCATION:
	case QUICK_ITEM_4_INDEX_LOCATION:
		return true;
	default:
		return false;
	}
}

/** Returns true if the index matches either of QA items indices */
inline bool IsQAItemIndex(int32 Index)
{
	switch (Index)
	{
	case QUICK_ITEM_1_INDEX_LOCATION:
	case QUICK_ITEM_2_INDEX_LOCATION:
	case QUICK_ITEM_3_INDEX_LOCATION:
	case QUICK_ITEM_4_INDEX_LOCATION:
		return true;
	default:
		return false;
	}
}