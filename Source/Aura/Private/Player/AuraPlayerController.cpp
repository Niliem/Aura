// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"

AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true;

    Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay();

    check(AuraContext);

    if (auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        InputSubsystem->AddMappingContext(AuraContext, 0);
    }

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();
}

void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent))
    {
        AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
        AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
    }
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB))
    {
        bTargeting = CurrentTarget ? true : false;
        bAutoRunning = false;
    }

    if (GetAuraAbilitySystemComponent() != nullptr)
    {
        GetAuraAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
    }
    GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (GetAuraAbilitySystemComponent() != nullptr)
    {
        GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
    }
    GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB))
    {
        if (GetAuraAbilitySystemComponent() != nullptr)
        {
            GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
        }
        return;
    }
    if (bTargeting)
    {
        if (GetAuraAbilitySystemComponent() != nullptr)
        {
            GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
        }
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();

        FHitResult CursorHit;
        if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
        {
            CachedDestination = CursorHit.ImpactPoint;
        }

        if (auto ControlledPawn = GetPawn<APawn>())
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection);

        }
    }

    GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Green, *InputTag.ToString());
}

void AAuraPlayerController::CursorTrace()
{
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    if (!CursorHit.bBlockingHit)
        return;

    LastTarget = CurrentTarget;
    CurrentTarget = CursorHit.GetActor();

    if (LastTarget == nullptr)
    {
        if (CurrentTarget != nullptr)
        {
            CurrentTarget->HighlightActor();
        }
    }
    else
    {
        if (CurrentTarget == nullptr)
        {
            LastTarget->UnHighlightActor();
        }
        else
        {
            if (LastTarget != CurrentTarget)
            {
                LastTarget->UnHighlightActor();
                CurrentTarget->HighlightActor();
            }
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
