// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"

#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UAuraInputConfig;
class IEnemyInterface;
class UAuraAbilitySystemComponent;
class USplineComponent;

struct FInputActionValue;

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AAuraPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UAuraInputConfig> InputConfig;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> AuraContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> ShiftAction;

    void Move(const FInputActionValue& InputActionValue);
    void ShiftPressed()
    {
        bShiftPressed = true;
    }
    void ShiftReleased()
    {
        bShiftPressed = false;
    }
    bool bShiftPressed = false;

    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);
    void AbilityInputTagHeld(FGameplayTag InputTag);

    void TraceTargetUnderCursor();
    TScriptInterface<IEnemyInterface> LastTarget;
    TScriptInterface<IEnemyInterface> CurrentTarget;

    FHitResult CursorHit;

    UPROPERTY()
    TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

    UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();

    FVector CachedDestination = FVector::ZeroVector;
    float FollowTime = 0.0f;
    float ShortPressThreshold = 0.5f;
    bool bAutoRunning = false;
    float AutoRunAcceptanceRadius = 50.0f;
    bool bTargeting = false;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USplineComponent> Spline;

    void AutoRun();
};
