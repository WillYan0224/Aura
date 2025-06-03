// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Character/AuraCharacter.h"
#include "Components/SplineComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/CharacterMovementComponent.h"


AAuraPlayerController::AAuraPlayerController()
{
	// Enable replication: data is synchronized from server to client. 1-Way transfer
	// Clients cannot modify replicated properties, as the server is authoritative
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	// TScriptInterface
	LastActor = CurrentActor;
	CurrentActor = CursorHit.GetActor();
	/*
	 * Line Trace from cursor
	 * A. LastActor == null && CurrentActor == null
	 *		- Do nothing
	 * B. LastActor == null && CurrentActor != null
	 * 		- Call HighlightActor on CurrentActor
	 * C. LastActor != null && CurrentActor == null
	 * 		- Call UnhighlightActor on LastActor
	 * D. LastActor != null && CurrentActor != null && LastActor != CurrentActor (Both valid)
	 * 		- Call UnhighlightActor on LastActor
	 * 		- Call HighlightActor on CurrentActor
	 * E. LastActor != null && CurrentActor != null && LastActor == CurrentActor (Both valid)
	 * 		- Do nothing
	 */
	if (LastActor == nullptr)
	{
		if (CurrentActor != nullptr)
		{
			// case B
			CurrentActor->HighlightActor();
		}
		// case A - Do nothing
	}
	else // Both Valid
	{
		if (CurrentActor == nullptr)
		{
			// case C
			LastActor->UnhighlightActor();
		}
		else
		{
			if (LastActor != CurrentActor)
			{
				// case D
				LastActor->UnhighlightActor();
				CurrentActor->HighlightActor();
			}
		// case E - Do nothing
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(1, 2.5f, FColor::Red, *InputTag.ToString());
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = CurrentActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetAuraAbilitySystemComponent()) return;
	GetAuraAbilitySystemComponent()->AbilityLocalInputReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent())
		{
			GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	if (bTargeting)
	{
		if (GetAuraAbilitySystemComponent())
		{
			GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		// should be locked for RMB < Will see >
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult CursorHit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
		{
			if (CursorHit.bBlockingHit)
			{
				CachedDestination = CursorHit.ImpactPoint;
			}
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection, 1.0f);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	
	return AuraAbilitySystemComponent;
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraEnhancedInputComponent* AuraInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Jump);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	APawn* ControlledPawn = GetPawn<APawn>();
	ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
	if (ControlledCharacter && ControlledCharacter->GetCharacterMovement()->IsFalling())
	{
		return; // Ignore input if character is falling
	}
	
	const FVector2D& InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (ControlledPawn)
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::Jump()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->Jump();
	}
}

