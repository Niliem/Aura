// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "AuraAbilityTypes.generated.h"

/**
 *
 */
class UAbilitySystemComponent;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
    GENERATED_BODY()

    FDamageEffectParams(){}

    UPROPERTY()
    TObjectPtr<UObject> WorldContextObject = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    TSubclassOf<UGameplayEffect> DamageGameplayEffectClass;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float BaseDamage = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float AbilityLevel = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DeathImpulseMagnitude = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector DeathImpulse = FVector::ZeroVector;

    UPROPERTY(EditDefaultsOnly, Category = "Damage", Meta = (Categories = "DamageType"))
    FGameplayTag DamageType;

    UPROPERTY(EditDefaultsOnly, Category = "Damage|Debuff")
    float DebuffChance = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage|Debuff")
    float DebuffDamage = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage|Debuff")
    float DebuffFrequency = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage|Debuff")
    float DebuffDuration = 0.0f;
};
