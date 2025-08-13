// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Character/AuraCharacter.h"
#include "Components/SplineComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/DamageTextComponent.h"


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
	AutoRun();
}

void AAuraPlayerController::ShowDamageNumbers_Implementation(ACharacter* TargetCharacter, float DamageAmount)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		NewObject<UDamageTextComponent>(this, DamageTextComponentClass);
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->SetDamageText(DamageAmount);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}


void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction, 1.0f);

		// auto running tolerance
		const float DistanceToDestination = FVector::Dist(LocationOnSpline, CachedDestination);
		if (DistanceToDestination <= AutoRunAcceptanceRadius)	bAutoRunning = false;
	}
}

void AAuraPlayerController::CursorTrace()
{
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
	if (LastActor != CurrentActor)
	{
		if (LastActor) LastActor->UnhighlightActor();
		if (CurrentActor) CurrentActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(1, 2.5f, FColor::Red, *InputTag.ToString());
	{
		GEngine->AddOnScreenDebugMessage(1, 2.5f, FColor::Green, *InputTag.ToString());
		bTargeting = CurrentActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent())	GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetAuraAbilitySystemComponent())	GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	
	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn<APawn>();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLocation : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLocation, 10.f, 12, FColor::Green, false, 5.f);
				}
				// Prevent auto running if hit the NavMesh bound volume
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints.Last();
					bAutoRunning = true;
				}
			}
		}
		// reset tracking after release
		FollowTime = .0f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent())	GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		return;
	}
	
	if (bTargeting || bShiftKeyDown) // Targeting or holding shift-key
	{
		if (GetAuraAbilitySystemComponent())	GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
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
	if (AuraAbilitySystemComponent == nullptr)	AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
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
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	
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

