// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "UI/Widget/FloatingTextWidgetComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true;

    Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::ClientShowFloatingDamageNumber_Implementation(float DamageAmount, AActor* Target, bool bIsBlockedHit, bool bIsCriticalHit)
{
    if (IsValid(Target) && FloatingDamageTextComponentClass)
    {
        UFloatingTextWidgetComponent* DamageText = NewObject<UFloatingTextWidgetComponent>(Target, FloatingDamageTextComponentClass);
        DamageText->RegisterComponent();
        DamageText->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageText->SetFloatingTextFromNumber(DamageAmount);
        FGameplayTagContainer TagContaier;
        if (bIsBlockedHit)
            TagContaier.AddTag(AuraGameplayTags::Effect_BlockedHit);
        if (bIsCriticalHit)
            TagContaier.AddTag(AuraGameplayTags::Effect_CriticalHit);
        DamageText->SetGameplayTags(TagContaier);
        DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    }
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

    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    TraceTargetUnderCursor();
    AutoRun();
}

void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent))
    {
        AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
        AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
        AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
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

    if (GetAuraAbilitySystemComponent())
        GetAuraAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB) || bTargeting || bShiftPressed)
    {
        if (GetAuraAbilitySystemComponent())
            GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
    }
    else
    {
        const auto ControlledPawn = GetPawn<APawn>();
        if (FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                if (NavPath->PathPoints.Num() > 0)
                {
                    Spline->ClearSplinePoints();
                    for (auto& PointLocation : NavPath->PathPoints)
                    {
                        Spline->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
                    }
                    CachedDestination = NavPath->PathPoints.Last();
                    bAutoRunning = true;
                }
            }
        }
        FollowTime = 0.0f;
        bTargeting = false;
    }
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB) || bTargeting || bShiftPressed)
    {
        if (GetAuraAbilitySystemComponent())
            GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();

        if (CursorHit.bBlockingHit)
            CachedDestination = CursorHit.ImpactPoint;

        if (auto ControlledPawn = GetPawn<APawn>())
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection);
        }
    }
}

void AAuraPlayerController::AutoRun()
{
    if (!bAutoRunning)
        return;

    if (auto ControlledPawn = GetPawn<APawn>())
    {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
        const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
        if (DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bAutoRunning = false;
        }
    }
}

void AAuraPlayerController::TraceTargetUnderCursor()
{
    if (!CursorHit.bBlockingHit)
        return;

    LastTarget = CurrentTarget;
    CurrentTarget = CursorHit.GetActor();

    if (LastTarget != CurrentTarget)
    {
        if (LastTarget)
            LastTarget->UnHighlightActor();
        if (CurrentTarget)
            CurrentTarget->HighlightActor();
    }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
    if (AuraAbilitySystemComponent == nullptr)
        AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    return AuraAbilitySystemComponent;
}
