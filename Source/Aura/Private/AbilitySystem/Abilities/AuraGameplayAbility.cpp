// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"

UAuraGameplayAbility::UAuraGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    bServerRespectsRemoteAbilityCancellation = false;
}

void UAuraGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);

    if (UAuraAbilitySystemLibrary::GetAbilityTypeTagFromSpec(Spec).MatchesTagExact(AuraGameplayTags::Ability_Type_Passive))
    {
        if (UAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
        {
            SourceAbilitySystemComponent->TryActivateAbility(Spec.Handle);
        }
    }
}

float UAuraGameplayAbility::GetManaCostAtLevel(int32 Level) const
{
    float ManaCost = 0.0f;
    if (const auto CostEffect = GetCostGameplayEffect())
    {
        for (const auto Mod : CostEffect->Modifiers)
        {
            if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
            {
                Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
                return FMath::Abs(ManaCost);
            }
        }
    }

    return ManaCost;
}

float UAuraGameplayAbility::GetCooldownAtLevel(int32 Level) const
{
    float Cooldown = 0.0f;
    if (const auto CooldownEffect = GetCooldownGameplayEffect())
    {
        CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Cooldown);
    }
    return Cooldown;
}
