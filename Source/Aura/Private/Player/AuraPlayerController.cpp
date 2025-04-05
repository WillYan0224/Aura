// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/AuraCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	// Enable replication: data is synchronized from server to client. 1-Way transfer
	// Clients cannot modify replicated properties, as the server is authoritative
	bReplicates = true;
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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Jump);
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

