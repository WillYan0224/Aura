// Yan @2024


#include "Interaction/CombatInterface.h"

#include "GeometryCollection/GeometryCollectionComponent.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetPlayerLevel()
{
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation()
{
	return FVector();
}

