// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"
#include "WaitEffectTimeRemainingChange.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeRemainingChangeDelegate, float, TimeRemaining);

/**
 *
 */
UCLASS()
class AURA_API UWaitEffectTimeRemainingChange : public UBlueprintAsyncActionBase, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnTimeRemainingChangeDelegate Start;

    UPROPERTY(BlueprintAssignable)
    FOnTimeRemainingChangeDelegate Update;

    UPROPERTY(BlueprintAssignable)
    FOnTimeRemainingChangeDelegate End;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    static UWaitEffectTimeRemainingChange* WaitForEffectTimeRemainingChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag EffectTag);

    UFUNCTION(BlueprintCallable)
    void EndTask();

    // FTickableGameObject
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;

protected:
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    FGameplayTag InEffectTag;

    bool bShouldTick = false;

    void EffectTagChanged(const FGameplayTag EffectTag, int32 NewCount);
    float GetEffectTimeRemaining(const FGameplayTag& EffectTag) const;
};
