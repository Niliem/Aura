// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraDamageAbility : public UAuraGameplayAbility
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Damage", meta = (HidePin="Target"))
    float GetDamageAtLevel(const FGameplayTag& DamageType, int32 Level) const;

protected:
    UFUNCTION(BlueprintPure, Category = "Damage")
    FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle) const;

    UFUNCTION(BlueprintCallable, Category = "Damage")
    UAnimMontage* GetAbilityMontage() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Damage", Meta = (Categories = "DamageType"))
    FGameplayTag DamageType;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FScalableFloat DamageCurve;

    UPROPERTY(EditDefaultsOnly, Category = "Damage|Debuff")
    float DebuffChance = 20.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage|Debuff")
    float DebuffDamage = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage|Debuff")
    float DebuffFrequency = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage|Debuff")
    float DebuffDuration = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TArray<TObjectPtr<UAnimMontage>> AbilityMontages;
};
