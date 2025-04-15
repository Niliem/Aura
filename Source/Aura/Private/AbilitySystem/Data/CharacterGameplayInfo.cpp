// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/CharacterGameplayInfo.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const
{
    if (!IsValid(AbilitySystemComponent))
        return;

    if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
        return;

    for (const auto& GameplayAbilitySet : GameplayAbilitySets)
    {
        FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(GameplayAbilitySet.GameplayAbility, GameplayAbilitySet.AbilityLevel);
        GameplayAbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraGameplayTags::Ability_Status_Unlocked);
        AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
    }
}

void UEffectSet::GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const
{
    if (!IsValid(AbilitySystemComponent))
        return;

    if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
        return;

    const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

    for (const auto& GameplayEffectSet : GameplayEffectSets)
    {
        auto ContextHandle = AbilitySystemComponent->MakeEffectContext();
        ContextHandle.AddSourceObject(AvatarActor);
        const auto EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectSet.GameplayEffect, GameplayEffectSet.EffectLevel, ContextHandle);
        AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
    }
}

void UAbilityInputBindings::BindAbilityInputs(UAbilitySystemComponent* AbilitySystemComponent) const
{
    if (!IsValid(AbilitySystemComponent))
        return;

    if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
        return;

    if (const auto& AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        for (const auto& AbilityInputBinding : AbilityInputBindings)
        {
            if (AbilityInputBinding.InputTag.IsValid())
            {
                AuraAbilitySystemComponent->EquipAbility(AbilityInputBinding.AbilityTag, AbilityInputBinding.InputTag);
            }
        }
    }
}

void UCharacterGameplayInfo::InitializeGameplayInfo(UAbilitySystemComponent* AbilitySystemComponent)
{
    if (!IsValid(AbilitySystemComponent))
        return;

    if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
        return;

    for (const auto& GrantedAbilitySet : GrantedAbilitySets)
    {
        GrantedAbilitySet->GiveToAbilitySystem(AbilitySystemComponent);
    }

    for (const auto& GrantedEffectSet : GrantedEffectSets)
    {
        GrantedEffectSet->GiveToAbilitySystem(AbilitySystemComponent);
    }

    if (IsValid(AbilityInputBinding))
    {
        AbilityInputBinding->BindAbilityInputs(AbilitySystemComponent);
    }

    if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        AuraAbilitySystemComponent->bStartupAbilitiesGiven = true;
        AuraAbilitySystemComponent->OnAbilitiesGiven.Broadcast();
    }
}

int32 UCharacterGameplayInfo::GetXPReward(int32 Level) const
{
    if (!XPReward.Curve.IsNull())
        return static_cast<int32>(XPReward.GetValueAtLevel(Level));
    return 0;
}
