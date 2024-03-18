// Copyright Emircan Birbil


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority())
	{
		check(AuraContext);
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		check(Subsystem);
		Subsystem->AddMappingContext(AuraContext, 0);

		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Default;
		
		FInputModeGameAndUI InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetHideCursorDuringCapture(false);
		SetInputMode(InputModeData);
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = FRotator::ZeroRotator;
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}
	
	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace from cursor, several scenarios:
	 *	A. LastActor is null && CurrentActor is null
	 *		- Do nothing.
	 *	B. LastActor is null && CurrentActor is valid
	 *		- Highlight ThisActor.
	 *	C. LastActor is valid && CurrentActor is null
	 *		- UnHighlight LastActor.
	 *	D. Both actors are valid but LastActor != CurrentActor
	 *		- UnHighlight LastActor and Highlight CurrentActor.
	 *	E. Both actors are valid but LastActor == CurrentActor.
	 *		- Do nothing.
	 */
	if (LastActor == CurrentActor ||
		(LastActor == nullptr && CurrentActor == nullptr))
	{
		// Case A or E, do nothing.
		return;
	}
	
	if (LastActor == nullptr)
	{
		// Case B
		CurrentActor->HighlightActor();
	}
	else // LastActor is valid
	{
		LastActor->UnHighlightActor();
		if (CurrentActor != nullptr)
		{
			// Case D
			CurrentActor->HighlightActor();
		}
	}
}
