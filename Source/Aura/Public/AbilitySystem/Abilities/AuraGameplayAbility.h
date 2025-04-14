// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "AuraGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UAuraGameplayAbility();

    virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

    UFUNCTION(BlueprintNativeEvent)
    FString GetDescription(const int32 Level);

    UFUNCTION(BlueprintNativeEvent)
    FString GetNextLevelDescription(const int32 Level);

    static FString GetLockedDescription(const int32 Level);
};
