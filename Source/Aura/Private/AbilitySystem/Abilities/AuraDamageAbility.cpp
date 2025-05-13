// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

FGameplayEffectSpecHandle UAuraDamageAbility::MakeDamageEffectSpecHandle(FGameplayEffectContextHandle& ContextHandle) const
{
    const UAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    const FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);

    if (DamageType.IsValid() && DamageCurve.IsValid())
    {
        const float ScaledDamage = GetDamageAtLevel(DamageType, GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, DamageType, ScaledDamage);
    }

    return DamageEffectSpecHandle;
}

float UAuraDamageAbility::GetDamageAtLevel(const FGameplayTag& InDamageType, int32 Level) const
{
    if (DamageType.IsValid() && DamageType.MatchesTagExact(InDamageType))
    {
        if (DamageCurve.IsValid())
        {
            return DamageCurve.GetValueAtLevel(Level);
        }
    }
    return 0.0f;
}

FDamageEffectParams UAuraDamageAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
    FDamageEffectParams DamageParams;
    DamageParams.WorldContextObject = GetAvatarActorFromActorInfo();
    DamageParams.DamageGameplayEffectClass = DamageEffectClass;
    DamageParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    DamageParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    DamageParams.BaseDamage = GetDamageAtLevel(DamageType, GetAbilityLevel());
    DamageParams.AbilityLevel = GetAbilityLevel();
    DamageParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
    DamageParams.DamageType = DamageType;
    DamageParams.DebuffChance = DebuffChance;
    DamageParams.DebuffDamage = DebuffDamage;
    DamageParams.DebuffFrequency = DebuffFrequency;
    DamageParams.DebuffDuration = DebuffDuration;

    return DamageParams;
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
