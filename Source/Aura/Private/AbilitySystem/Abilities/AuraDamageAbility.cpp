// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

FGameplayEffectSpecHandle UAuraDamageAbility::MakeDamageEffectSpecHandle(FGameplayEffectContextHandle& ContextHandle) const
{
    const UAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    const FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);

    for (const auto& Damage : DamageTypes)
    {
        const float ScaledDamage = Damage.Value.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, Damage.Key, ScaledDamage);
    }
    return DamageEffectSpecHandle;
}

float UAuraDamageAbility::GetDamageAtLevel(const FGameplayTag& DamageType, int32 Level)
{
    if (const auto ScalableDamage = DamageTypes.Find(DamageType))
    {
        return ScalableDamage->GetValueAtLevel(Level);
    }
    return 0.0f;
}

UAnimMontage* UAuraDamageAbility::GetAbilityMontage() const
{
    if (AbilityMontages.Num() <= 0)
        return nullptr;

    int32 RandomIndex = FMath::RandRange(0, AbilityMontages.Num() - 1);
    if (AbilityMontages.IsValidIndex(RandomIndex))
        return AbilityMontages[RandomIndex];

    return nullptr;
}
