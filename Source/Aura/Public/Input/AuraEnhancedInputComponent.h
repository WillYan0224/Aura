// Yan @2024

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "AuraEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* UserObject, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

// template func
template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraEnhancedInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* UserObject,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const FAuraInputAction& InputAction : InputConfig->AbilityInputActions)
	{
		if (InputAction.Action && InputAction.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputAction.Action, ETriggerEvent::Started, UserObject, PressedFunc, InputAction.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(InputAction.Action, ETriggerEvent::Completed, UserObject, ReleasedFunc, InputAction.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(InputAction.Action, ETriggerEvent::Triggered, UserObject, HeldFunc, InputAction.InputTag);
			}
		}
	}
}
