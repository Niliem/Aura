// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
    None,
    AuraHero,
    Caster,
    Warrior,
    Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
    TSubclassOf<UGameplayEffect> PrimaryAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
    TSubclassOf<UGameplayEffect> SecondatyAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
    TSubclassOf<UGameplayEffect> VitalAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
    TMap<TSubclassOf<UGameplayAbility>, bool> Abilities;
};

/**
 *
 */
UCLASS(BlueprintType)
class AURA_API UCharacterClassInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    TSubclassOf<UGameplayEffect> GetPrimaryAttributesForClass(ECharacterClass CharacterClass) const;
    TSubclassOf<UGameplayEffect> GetSecondatyAttributesForClass(ECharacterClass CharacterClass) const;
    TSubclassOf<UGameplayEffect> GetVitalAttributesForClass(ECharacterClass CharacterClass) const;
    TMap<TSubclassOf<UGameplayAbility>, bool> GetAbilitiesForClass(ECharacterClass CharacterClass) const;
    TMap<TSubclassOf<UGameplayAbility>, bool> GetCommonAbilities() const;
    TObjectPtr<UCurveTable> GetDamageCalculationCoefficients() const;

private:
    FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass) const;

    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
    TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformations;

    UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
    TSubclassOf<UGameplayEffect> SecondatyAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
    TSubclassOf<UGameplayEffect> VitalAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
    TMap<TSubclassOf<UGameplayAbility>, bool> CommonAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
    TObjectPtr<UCurveTable> DamageCalculationCoefficients;
};
