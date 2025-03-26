// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
        return;

    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
        {
            AbilitySpecInputPressed(AbilitySpec);
            if (!AbilitySpec.IsActive())
            {
                for (const UGameplayAbility* AbilityInstance : AbilitySpec.GetAbilityInstances())
                {
                    InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilityInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
                }
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
        if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
        {
            AbilitySpecInputReleased(AbilitySpec);
            if (AbilitySpec.IsActive())
            {
                for (const UGameplayAbility* AbilityInstance : AbilitySpec.GetAbilityInstances())
                {
                    InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilityInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
                }
            }
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
        return;

    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
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

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        for (auto Tag : AbilitySpec.Ability.Get()->GetAssetTags())
        {
            if (!Tag.MatchesTag(AuraGameplayTags::Ability_Status) && !Tag.MatchesTag(AuraGameplayTags::Ability_Type))
            {
                if (Tag.MatchesTag(AuraGameplayTags::Ability))
                {
                    return Tag;
                }
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (auto Tag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if (Tag.MatchesTag(AuraGameplayTags::Ability_Status))
        {
            return Tag;
        }
    }
    if (AbilitySpec.Ability)
    {
        for (auto Tag : AbilitySpec.Ability.Get()->GetAssetTags())
        {
            if (Tag.MatchesTag(AuraGameplayTags::Ability_Status))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTypeFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (auto Tag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if (Tag.MatchesTag(AuraGameplayTags::Ability_Type))
        {
            return Tag;
        }
    }
    if (AbilitySpec.Ability)
    {
        for (auto Tag : AbilitySpec.Ability.Get()->GetAssetTags())
        {
            if (Tag.MatchesTag(AuraGameplayTags::Ability_Type))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (auto Tag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if (Tag.MatchesTag(AuraGameplayTags::InputTag))
        {
            return Tag;
        }
    }
    if (AbilitySpec.Ability)
    {
        for (auto Tag : AbilitySpec.Ability.Get()->GetAssetTags())
        {
            if (Tag.MatchesTag(AuraGameplayTags::InputTag))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        for (const FGameplayTag Tag: AbilitySpec.Ability.Get()->GetAssetTags())
        {
            if (Tag.MatchesTagExact(AbilityTag))
            {
                return &AbilitySpec;
            }
        }
    }

    return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeEventTag)
{
    ServerUpgradeAttribute(AttributeEventTag);
}

void UAuraAbilitySystemComponent::SpendSpellPoint(const FGameplayTag& AbilityTag)
{
    if (AbilityTag.IsValid())
    {
        ServerSpendSpellPoint(AbilityTag);
    }
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(const int32 Level)
{
    if (const auto AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor()))
    {
        for (const auto& Info : AbilityInfo->AbilityInformation)
        {
            if (!Info.AbilityTag.IsValid())
                continue;
            if (Level < Info.RequirementLevel)
                continue;

            if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
            {
                FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
                AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraGameplayTags::Ability_Status_Eligible);
                GiveAbility(AbilitySpec);
                MarkAbilitySpecDirty(AbilitySpec);

                ClientUpdateAbilityStatus(Info.AbilityTag, AuraGameplayTags::Ability_Status_Eligible, 1);
            }
        }
    }
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

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
    if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
    {
        FGameplayEventData SpellPointsPayload;
        SpellPointsPayload.EventTag = AuraGameplayTags::GameplayEvent_SpellPoints;
        SpellPointsPayload.EventMagnitude = -1.0f;
        FScopedPredictionWindow SpellPointsWindow(this, true);
        HandleGameplayEvent(SpellPointsPayload.EventTag, &SpellPointsPayload);

        FGameplayTag Status = GetAbilityStatusFromSpec(*AbilitySpec);
        if (Status.MatchesTagExact(AuraGameplayTags::Ability_Status_Eligible))
        {
            AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(AuraGameplayTags::Ability_Status_Eligible);
            AbilitySpec->GetDynamicSpecSourceTags().AddTag(AuraGameplayTags::Ability_Status_Unlocked);

            Status = AuraGameplayTags::Ability_Status_Unlocked;
        }
        else if (Status.MatchesTagExact(AuraGameplayTags::Ability_Status_Equipped) || Status.MatchesTagExact(AuraGameplayTags::Ability_Status_Unlocked))
        {
            AbilitySpec->Level++;
        }

        ClientUpdateAbilityStatus(AbilityTag, Status, 1);

        MarkAbilitySpecDirty(*AbilitySpec);
    }
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 Level)
{
    OnAbilityStatusChanged.Broadcast(AbilityTag, StatusTag, Level);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);
    EffectAssetTags.Broadcast(TagContainer);
}
