// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

FGameplayEffectSpecHandle UAuraDamageAbility::MakeDamageEffectSpecHandle(FGameplayEffectContextHandle& ContextHandle) const
{
    const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
    const FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);

    for (const auto& Damage : DamageTypes)
    {
        const float ScaledDamage = Damage.Value.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, Damage.Key, ScaledDamage);
    }
    return DamageEffectSpecHandle;
}

FTaggedMontage UAuraDamageAbility::GetDamageMontage() const
{
    if (DamageMontages.Num() <= 0)
        return FTaggedMontage();

    int32 RandomIndex = FMath::RandRange(0, DamageMontages.Num() - 1);
    if (DamageMontages.IsValidIndex(RandomIndex))
        return DamageMontages[RandomIndex];

    return FTaggedMontage();
}
