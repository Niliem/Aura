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

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
    if (const auto AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
    {
        const auto AbilityStatus = UAuraAbilitySystemLibrary::GetAbilityStatusTagFromSpec(*AbilitySpec);
        if (AbilityStatus.MatchesTagExact(AuraGameplayTags::Ability_Status_Eligible))
        {
            OutDescription = UAuraAbilitySystemLibrary::GetAbilityDescription(this, AbilityTag, AbilitySpec->Level, EAbilityDescriptionType::Default);
            OutNextLevelDescription = FString();
            return true;
        }
        else
        {
            OutDescription = UAuraAbilitySystemLibrary::GetAbilityDescription(this, AbilityTag, AbilitySpec->Level, EAbilityDescriptionType::Default);
            OutNextLevelDescription = UAuraAbilitySystemLibrary::GetAbilityDescription(this, AbilityTag, AbilitySpec->Level, EAbilityDescriptionType::Upgrade);
            return true;
        }
    }
    if (const auto AbilityInfo = UAuraAbilitySystemLibrary::GetAbilitiesInfo(GetAvatarActor()))
    {
        OutDescription = UAuraAbilitySystemLibrary::GetAbilityDescription(this, AbilityTag, 1, EAbilityDescriptionType::Require);
        OutNextLevelDescription = FString();
        return false;
    }
    OutDescription = FString();
    OutNextLevelDescription = FString();
    return false;
}

void UAuraAbilitySystemComponent::AssignAbilityToInputTag(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
    if (const auto AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
    {
        AssignAbilityToInputTag(*AbilitySpec, InputTag);
    }
}

void UAuraAbilitySystemComponent::AssignAbilityToInputTag(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& InputTag)
{
    const FGameplayTag& AbilityStatus = UAuraAbilitySystemLibrary::GetAbilityStatusTagFromSpec(AbilitySpec);
    const bool bCanEquipAbility = (AbilityStatus.MatchesTagExact(AuraGameplayTags::Ability_Status_Equipped) || AbilityStatus.MatchesTagExact(AuraGameplayTags::Ability_Status_Unlocked));
    if (bCanEquipAbility)
    {
        ClearAbilitiesFromInputTag(InputTag);

        const FGameplayTag& PrevInputTag = ClearAbilityInputTag(&AbilitySpec);
        AbilitySpec.GetDynamicSpecSourceTags().AddTag(InputTag);

        if (AbilityStatus.MatchesTagExact(AuraGameplayTags::Ability_Status_Unlocked))
        {
            AbilitySpec.GetDynamicSpecSourceTags().RemoveTag(AuraGameplayTags::Ability_Status_Unlocked);
            AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraGameplayTags::Ability_Status_Equipped);
        }

        MarkAbilitySpecDirty(AbilitySpec);
        ClientEquipAbility(UAuraAbilitySystemLibrary::GetAbilityTagFromSpec(AbilitySpec), InputTag, PrevInputTag);
    }
}

FGameplayTag UAuraAbilitySystemComponent::ClearAbilityInputTag(FGameplayAbilitySpec* AbilitySpec)
{
    const FGameplayTag& InputTag = UAuraAbilitySystemLibrary::GetAbilityInputTagFromSpec(*AbilitySpec);
    AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(InputTag);
    MarkAbilitySpecDirty(*AbilitySpec);
    return InputTag;
}

void UAuraAbilitySystemComponent::ClearAbilitiesFromInputTag(const FGameplayTag& InputTag)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        const FGameplayTag& CurrentInputTag = UAuraAbilitySystemLibrary::GetAbilityInputTagFromSpec(AbilitySpec);
        if (InputTag.MatchesTagExact(CurrentInputTag))
        {
            ClearAbilityInputTag(&AbilitySpec);
        }
    }
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

void UAuraAbilitySystemComponent::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
    if (AbilityTag.IsValid())
    {
        ServerEquipAbility(AbilityTag, InputTag);
    }
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(const int32 Level)
{
    if (const auto AbilityInfo = UAuraAbilitySystemLibrary::GetAbilitiesInfo(GetAvatarActor()))
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

void UAuraAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
    Super::OnGiveAbility(AbilitySpec);

    OnAbilityGiven.Broadcast(UAuraAbilitySystemLibrary::GetAbilityTagFromSpec(AbilitySpec));
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

        FGameplayTag Status = UAuraAbilitySystemLibrary::GetAbilityStatusTagFromSpec(*AbilitySpec);
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

        MarkAbilitySpecDirty(*AbilitySpec);
        ClientUpdateAbilityStatus(AbilityTag, Status, 1);
    }
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
    AssignAbilityToInputTag(AbilityTag, InputTag);
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 Level)
{
    OnAbilityStatusChanged.Broadcast(AbilityTag, StatusTag, Level);
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag)
{
    OnAbilityEquipped.Broadcast(AbilityTag, InputTag, PrevInputTag);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayHandle)
{
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);
    EffectAssetTags.Broadcast(TagContainer);
}
