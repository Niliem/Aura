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
    TArray<TSubclassOf<UGameplayAbility>> Abilities;
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
    TArray<TSubclassOf<UGameplayAbility>> GetAbilitiesForClass(ECharacterClass CharacterClass) const;
    TArray<TSubclassOf<UGameplayAbility>> GetCommonAbilities() const;
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
    TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
    TObjectPtr<UCurveTable> DamageCalculationCoefficients;
};
