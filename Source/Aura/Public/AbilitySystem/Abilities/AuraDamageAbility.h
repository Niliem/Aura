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

protected:
    UFUNCTION(BlueprintPure, Category = "Damage")
    FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle) const;

    UFUNCTION(BlueprintCallable, Category = "Damage")
    UAnimMontage* GetDamageMontage() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> DamageTypes;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TArray<TObjectPtr<UAnimMontage>> DamageMontages;
};
