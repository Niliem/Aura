// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraGameplayTags.h"
#include "Aura/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    // OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ServerEffectApplied);
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

void UAuraAbilitySystemComponent::ExecuteActivePeriodicEffectsWithTags(const FGameplayTagContainer& Tags)
{
    const auto ActiveEffectHandles = GetActiveEffectsWithAllTags(Tags);
    for (const auto& ActiveEffectHandle : ActiveEffectHandles)
    {
        ExecuteActivePeriodicEffect(ActiveEffectHandle);
    }
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for (const auto& AbilitySpec : GetActivatableAbilities())
    {
        if (!Delegate.ExecuteIfBound(AbilitySpec))
        {
            UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
        }
    }
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec) const
{
    if (AbilitySpec.Ability)
    {
        for (auto Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if (Tag.MatchesTag(AuraGameplayTags::Ability))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec) const
{
    for (auto Tag : AbilitySpec.DynamicAbilityTags)
    {
        if (Tag.MatchesTag(AuraGameplayTags::InputTag))
        {
            return Tag;
        }
    }
    return FGameplayTag();
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeEventTag)
{
    ServerUpgradeAttribute(AttributeEventTag);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

    if (!bStartupAbilitiesGiven)
    {
        bStartupAbilitiesGiven = true;
        OnAbilitiesGiven.Broadcast();
    }
}

void UAuraAbilitySystemComponent::ExecuteActivePeriodicEffect(const FActiveGameplayEffectHandle Handle)
{
    if (const auto ActiveEffect = GetActiveGameplayEffect(Handle))
    {
        if (ActiveEffect->GetPeriod() > UGameplayEffect::NO_PERIOD)
        {
            ExecutePeriodicEffect(Handle);
        }
    }
}

void UAuraAbilitySystemComponent::ServerEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);

    MulticastEffectApplied(TagContainer);
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeEventTag)
{
    FGameplayEventData AttributePayload;
    AttributePayload.EventTag = AttributeEventTag;
    AttributePayload.EventMagnitude = 1.0f;
    FScopedPredictionWindow AttributeWindow(this, true);
    HandleGameplayEvent(AttributePayload.EventTag, &AttributePayload);

    FGameplayEventData AttributePointsPayload;
    AttributePointsPayload.EventTag = AuraGameplayTags::GameplayEvent_AttributePoints;
    AttributePointsPayload.EventMagnitude = -1.0f;
    FScopedPredictionWindow AttributePointsWindow(this, true);
    HandleGameplayEvent(AttributePointsPayload.EventTag, &AttributePointsPayload);
}

void UAuraAbilitySystemComponent::MulticastEffectApplied_Implementation(FGameplayTagContainer EffectTags)
{
    EffectAssetTags.Broadcast(EffectTags);
}
