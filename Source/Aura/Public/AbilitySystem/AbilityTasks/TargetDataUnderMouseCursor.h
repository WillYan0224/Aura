// Yan @2024

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouseCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseCursorTargetDataDelegate, const FGameplayAbilityTargetDataHandle&, DataHandle);
/**
 Custom AbilityTask reference from Epic's GAS Library 
 */

struct FGameplayAbilityTargetDataHandle;

UCLASS()
class AURA_API UTargetDataUnderMouseCursor : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FMouseCursorTargetDataDelegate OnMouseCursor;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = ( DisplayName = "TargetDataUnderMouseCursor", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UTargetDataUnderMouseCursor* CreateTargetDataUnderMouseCursor(UGameplayAbility* OwningAbility);

private:

	virtual void Activate() override;

	void SendMouseCursorData();
	
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
