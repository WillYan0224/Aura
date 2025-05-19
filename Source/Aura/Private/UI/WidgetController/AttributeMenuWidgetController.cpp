// Yan @2024


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"


void UAttributeMenuWidgetController::BindCallbacksToDependancies()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AuraAttributeSet](const FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AttributeSet);
				AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AuraAttributeSet->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}
	
}
