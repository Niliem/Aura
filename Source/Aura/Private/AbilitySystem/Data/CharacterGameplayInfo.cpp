// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/CharacterGameplayInfo.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UCharacterGameplayInfo::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent) const
{
    if (!IsValid(AbilitySystemComponent))
        return;

    if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
        return;

    const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

    for (const auto& AbilitySet : GrantedAbilitySets)
    {
        FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilitySet.Ability, AbilitySet.AbilityLevel);
        GameplayAbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
        if (AbilitySet.ActivateOnGranted)
        {
            AbilitySystemComponent->GiveAbilityAndActivateOnce(GameplayAbilitySpec);
        }
        else
        {
            AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
        }
    }
}

void UCharacterGameplayInfo::GiveEffects(UAbilitySystemComponent* AbilitySystemComponent) const
{
    if (!IsValid(AbilitySystemComponent))
        return;

    if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
        return;

    const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

    for (const auto& Effect : GrantedEffects)
    {
        auto ContextHandle = AbilitySystemComponent->MakeEffectContext();
        ContextHandle.AddSourceObject(AvatarActor);
        const auto EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.0f, ContextHandle);
        AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
    }
}
