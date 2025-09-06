// Yan @2024

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraAbilitySystemLibrary.generated.h"


class UAbilitySystemComponent;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemBlueprintLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemBlueprintLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemBlueprintLibrary|CharacterClassDefault")
	static void InitializeDefaultAttribute(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass, float level);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemBlueprintLibrary|CharacterClassDefault")
	static void GiveStartUpAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemBlueprintLibrary|CharacterClassDefault")
	static UCharacterClassInfo *GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemBlueprintLibrary|GameplayEffect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);	

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemBlueprintLibrary|GameplayEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemBlueprintLibrary|GameplayEffect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlocked);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemBlueprintLibrary|GameplayEffect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInCriticalHit);
	
};

