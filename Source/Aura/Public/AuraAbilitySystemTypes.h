#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilitySystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
    /** Returns the actual struct used for serialization, subclasses must override this! */
    virtual UScriptStruct* GetScriptStruct() const override
    {
        return FAuraGameplayEffectContext::StaticStruct();
    }
    /** Custom serialization, subclasses must override this */
    virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
    
    bool IsCriticalHit() const { return bIsCriticalHit; }
    bool IsBlockedHit() const  { return bIsBlockedHit;  }

    void SetIsCriticalHit(bool bInIsCriticalHit)  { bIsCriticalHit = bInIsCriticalHit; };
    void SetIsBlockedHit(bool bInIsBlockedHit)    { bIsBlockedHit = bInIsBlockedHit; };
    
protected:

    UPROPERTY()
    bool bIsBlockedHit = false;

    UPROPERTY()
    bool bIsCriticalHit = false;
};
