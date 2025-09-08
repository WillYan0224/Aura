// Yan @2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * POD-Style Singleton class to manage gameplay tags in the Aura project
 * Doesn't have lazy new instance creation, but it is not needed in this case
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Vigor;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Intelligence;

	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_DirectHitChance;
	FGameplayTag Attribute_Secondary_DirectHitDamage;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_MaxHealth;

	// Input tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	// Meta Damage
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;

	TArray<FGameplayTag> DamageTypes;
	
	FGameplayTag Effect_HitReact;
	
private:
	static FAuraGameplayTags GameplayTags;
	
protected:
	
};