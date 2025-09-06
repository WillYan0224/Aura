// Yan @2024


#include "AbilitySystem/AuraAbilitySystemGlobal.h"

#include "AuraAbilitySystemTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext();
}
