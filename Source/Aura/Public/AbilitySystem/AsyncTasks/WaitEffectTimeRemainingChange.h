// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "WaitEffectTimeRemainingChange.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeRemainingChangeDelegate, float, TimeRemaining);

/**
 *
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitEffectTimeRemainingChange : public UBlueprintAsyncActionBase
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

protected:
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    FGameplayTag InEffectTag;

    FTimerHandle UpdateTimeRemainingTimer;

    UFUNCTION()
    void EffectTagChanged(const FGameplayTag EffectTag, int32 NewCount);

    UFUNCTION()
    void UpdateTimeRemaining();

    float GetEffectTimeRemaining(const FGameplayTag& EffectTag) const;
};
