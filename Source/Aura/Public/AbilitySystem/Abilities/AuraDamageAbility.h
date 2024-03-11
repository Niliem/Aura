// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageAbility.generated.h"

USTRUCT(BlueprintType)
struct FTaggedMontage
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* Montage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag Tag;
};

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
    FTaggedMontage GetDamageMontage() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TMap<FGameplayTag, FScalableFloat> DamageTypes;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    TArray<FTaggedMontage> DamageMontages;

    UPROPERTY(BlueprintReadWrite, Category = "Damage")
    TObjectPtr<UAnimMontage> MontageToPlay = nullptr;

    UPROPERTY(BlueprintReadWrite, Category = "Damage")
    FGameplayTag SocketTag;
};
