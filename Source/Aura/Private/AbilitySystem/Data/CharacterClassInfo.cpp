// Yan @2024


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultData UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInfo.FindChecked(CharacterClass);
}
