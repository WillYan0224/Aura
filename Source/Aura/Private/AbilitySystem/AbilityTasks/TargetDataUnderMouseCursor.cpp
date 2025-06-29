// Yan @2024


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouseCursor.h"

#include "AbilitySystemComponent.h"


// -- Take reference from AbilityTask_PlayMontageAndWait.cpp/.h -- 
UTargetDataUnderMouseCursor* UTargetDataUnderMouseCursor::CreateTargetDataUnderMouseCursor(
	UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouseCursor* MyObj = NewAbilityTask<UTargetDataUnderMouseCursor>(OwningAbility);

	return MyObj;
}

void UTargetDataUnderMouseCursor::Activate()
{
	// DO NOT CALL SUPER ON THIS (Since it's not even overriden in AbilityTask.h -- Epic's code base

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey PredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this, &UTargetDataUnderMouseCursor::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouseCursor::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	// -- Send MouseCursor Target Data to server and try replicated it from server to local
	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
    PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnMouseCursor.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouseCursor::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,	FGameplayTag ActivationTag)
{
	// Flush Target Data and clear the cache
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnMouseCursor.Broadcast(DataHandle);
	}
}
