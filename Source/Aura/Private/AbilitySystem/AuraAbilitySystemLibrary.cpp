// Yan @2024


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState ? PlayerState->GetAbilitySystemComponent() : nullptr;
			UAttributeSet* AttributeSet = PlayerState ? PlayerState->GetAttributeSet() : nullptr;
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			if (AbilitySystemComponent && AttributeSet)
			{
				return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
			}
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState ? PlayerState->GetAbilitySystemComponent() : nullptr;
			UAttributeSet* AttributeSet = PlayerState ? PlayerState->GetAttributeSet() : nullptr;
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			if (AbilitySystemComponent && AttributeSet)
			{
				return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
			}
		}
	}
	return nullptr;
}


