// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass) const
{
    return CharacterClassInformations.FindChecked(CharacterClass);
}

TSubclassOf<UGameplayEffect> UCharacterClassInfo::GetPrimaryAttributesForClass(ECharacterClass CharacterClass) const
{
    return GetClassDefaultInfo(CharacterClass).PrimaryAttributes;
}

TSubclassOf<UGameplayEffect> UCharacterClassInfo::GetSecondatyAttributesForClass(ECharacterClass CharacterClass) const
{
    const FCharacterClassDefaultInfo ClassDefaultInfo = GetClassDefaultInfo(CharacterClass);
    return (ClassDefaultInfo.SecondatyAttributes) ? ClassDefaultInfo.SecondatyAttributes : SecondatyAttributes;
}

TSubclassOf<UGameplayEffect> UCharacterClassInfo::GetVitalAttributesForClass(ECharacterClass CharacterClass) const
{
    const FCharacterClassDefaultInfo ClassDefaultInfo = GetClassDefaultInfo(CharacterClass);
    return (ClassDefaultInfo.VitalAttributes) ? ClassDefaultInfo.VitalAttributes : VitalAttributes;
}

TMap<TSubclassOf<UGameplayAbility>, bool> UCharacterClassInfo::GetAbilitiesForClass(ECharacterClass CharacterClass) const
{
    const FCharacterClassDefaultInfo ClassDefaultInfo = GetClassDefaultInfo(CharacterClass);
    return ClassDefaultInfo.Abilities;
}

TMap<TSubclassOf<UGameplayAbility>, bool> UCharacterClassInfo::GetCommonAbilities() const
{
    return CommonAbilities;
}

TObjectPtr<UCurveTable> UCharacterClassInfo::GetDamageCalculationCoefficients() const
{
    return DamageCalculationCoefficients;
}
