// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenDelegate);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    void AbilityActorInfoSet();

    void AbilityInputTagPressed(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag& InputTag);
    void AbilityInputTagHeld(const FGameplayTag& InputTag);

    UFUNCTION(BlueprintCallable, Category = "GameplayEffects")
    void ExecuteActivePeriodicEffectsWithTags(const FGameplayTagContainer& Tags);

    FEffectAssetTagsDelegate EffectAssetTags;
    FAbilitiesGivenDelegate OnAbilitiesGiven;

    bool bStartupAbilitiesGiven = false;

    void ForEachAbility(const FForEachAbility& Delegate);

    static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
    static FGameplayTag GetAbilityStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
    static FGameplayTag GetAbilityTypeFromSpec(const FGameplayAbilitySpec& AbilitySpec);
    static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

    void UpgradeAttribute(const FGameplayTag& AttributeEventTag);

protected:
    UFUNCTION(Server, Reliable)
    void ServerEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayHandle);

    UFUNCTION(Server, Reliable)
    void ServerUpgradeAttribute(const FGameplayTag& AttributeEventTag);

    UFUNCTION(NetMulticast, Unreliable)
    void MulticastEffectApplied(FGameplayTagContainer EffectTags);

    virtual void OnRep_ActivateAbilities() override;

private:
    void ExecuteActivePeriodicEffect(const FActiveGameplayEffectHandle Handle);
};
