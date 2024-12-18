// © 2024 Yan

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	// This is a macro that will generate a function called OnRep_Health MUST HAVE
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(ReplicatedUsing = Onrep_Mana, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	
	// In order for it to be a RepNotify, it must be a UFUNCTION
	// Argument has to be the same type as the attribute or 0 argument
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};



