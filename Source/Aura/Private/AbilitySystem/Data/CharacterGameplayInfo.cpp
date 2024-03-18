// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/CharacterGameplayInfo.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UCharacterGameplayInfo::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent, float Level) const
{
    if (!IsValid(AbilitySystemComponent))
        return;

    if (!AbilitySystemComponent->IsOwnerActorAuthoritative())
        return;

    for (const auto& AbilitySet : GrantedAbilitySets)
    {
        const float AbilityLevel = (AbilitySet.bIsScalable) ? Level : AbilitySet.AbilityLevel;
        FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilitySet.Ability, AbilityLevel);
        GameplayAbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
        if (AbilitySet.bActivateOnGranted)
        {
            AbilitySystemComponent->GiveAbilityAndActivateOnce(GameplayAbilitySpec);
        }
        else
        {
            AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
        }
    }

    if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        AuraAbilitySystemComponent->bStartupAbilitiesGiven = true;
        AuraAbilitySystemComponent->OnAbilitiesGiven.Broadcast();
    }
}

void UCharacterGameplayInfo::GiveEffects(UAbilitySystemComponent* AbilitySystemComponent, float Level) const
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
        const auto EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, Level, ContextHandle);
        AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
    }
}
