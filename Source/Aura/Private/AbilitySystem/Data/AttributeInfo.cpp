// Yan @2024


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInfomation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			if (bLogFound)
			{
				UE_LOG(LogTemp, Log, TEXT("Found attribute info for tag: %s"), *AttributeTag.ToString());
			}
			return Info;
		}
	}

	if (!bLogFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attribute info not found for tag: %s"), *AttributeTag.ToString());
	}
	return FAuraAttributeInfo();
}
