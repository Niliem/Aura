// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer&);

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    void AbilityActorInfoSet();

    FEffectAssetTagsDelegate EffectAssetTags;

    void AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

    void AbilityInputTagPressed(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag& InputTag);
    void AbilityInputTagHeld(const FGameplayTag& InputTag);

    UFUNCTION(BlueprintCallable, Category = "GameplayEffects")
    void ExecuteActivePeriodicEffectByTag(const FGameplayTag& Tag);

protected:
    UFUNCTION(Server, Reliable)
    void ServerEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayHandle);

    UFUNCTION(NetMulticast, Unreliable)
    void MulticastEffectApplied(FGameplayTagContainer EffectTags);

private:
    void ExecuteActivePeriodicEffect(const FActiveGameplayEffectHandle Handle);
};
