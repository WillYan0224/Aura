// Yan @2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobal.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobal : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
