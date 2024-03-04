// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ServerEffectApplied);
}

void UAuraAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
    for (auto& AbilityClass : Abilities)
    {
        FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability))
        {
            GameplayAbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->InputTag);
            GiveAbility(GameplayAbilitySpec);
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
        return;

    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if (!AbilitySpec.IsActive())
            {
                InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
            }
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
        return;

    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputReleased(AbilitySpec);
            InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
        return;

    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if (!AbilitySpec.IsActive())
            {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
    }
}

void UAuraAbilitySystemComponent::ServerEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);

    MulticastEffectApplied(TagContainer);
}


void UAuraAbilitySystemComponent::MulticastEffectApplied_Implementation(FGameplayTagContainer EffectTags)
{
    EffectAssetTags.Broadcast(EffectTags);
}
