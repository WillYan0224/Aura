// Yan @2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist UMETA(DisplayName = "Elementalist"),
	Warrior		 UMETA(DisplayName = "Warrior"),
	Ranger		 UMETA(DisplayName = "Ranger"),
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterClass Default")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterClass Default")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
};

/*
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TMap<ECharacterClass, FCharacterClassDefaultData> CharacterClassInfo;
	 
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default|Damage Calculation")
	TObjectPtr<UCurveTable> DamageCalculationCoefficientTable;
	
	
	FCharacterClassDefaultData GetClassDefaultInfo(ECharacterClass CharacterClass);
};
