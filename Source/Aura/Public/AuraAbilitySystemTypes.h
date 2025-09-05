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
    virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;  // For more info, Package stride & little/big endian; TLDR, data layout 

    /** Creates a copy of this context, used to duplicate for later modifications */
    virtual FAuraGameplayEffectContext* Duplicate() const
    {
        FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
        *NewContext = *this;
        if (GetHitResult())
        {
            // Does a deep copy of the hit result
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        return NewContext;
    }
    
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

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true       // Necessary so that TSharedPtr<FHitResult> Data is copied around
    };
};