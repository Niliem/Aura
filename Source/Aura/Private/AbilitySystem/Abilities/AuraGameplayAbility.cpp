// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

UAuraGameplayAbility::UAuraGameplayAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    bServerRespectsRemoteAbilityCancellation = false;
}

void UAuraGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);

    if (UAuraAbilitySystemComponent::GetAbilityTypeFromSpec(Spec).MatchesTagExact(AuraGameplayTags::Ability_Type_Passive))
    {
        if (UAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
        {
            SourceAbilitySystemComponent->TryActivateAbility(Spec.Handle);
        }
    }
}

FString UAuraGameplayAbility::GetDescription(const int32 Level)
{
    return FString::Printf(TEXT("<Default>%s, </><Level>%i</>"), L"Default ability name", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(const int32 Level)
{
    return FString::Printf(TEXT("<Default>Next level: %s, </><Level>%i</>"), L"Default ability name", Level);
}

FString UAuraGameplayAbility::GetLockedDescription(const int32 Level)
{
    return FString::Printf(TEXT("<Default>Spell locked until level: %i, </>"), Level);
}
