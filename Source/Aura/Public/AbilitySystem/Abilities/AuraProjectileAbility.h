// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageAbility.h"

#include "AuraProjectileAbility.generated.h"

class AAuraProjectileActor;
class UGameplayEffect;

/**
 *
 */
UCLASS()
class AURA_API UAuraProjectileAbility : public UAuraDamageAbility
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TSubclassOf<AAuraProjectileActor> ProjectileActorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float ProjectileSpread = 90.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    int32 NumProjectiles = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    int32 MaxNumProjectiles = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    bool bUseOverridePitch = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseOverridePitch", EditConditionHides), Category = "Projectile")
    float OverridePitch = 0.0f;

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void SpawnProjectile(const FVector& TargetLocation, FGameplayTag Socket, bool bOverridePitch = false, float PitchOverride = 0.0f);

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void SpawnProjectiles(const FVector& TargetLocation, FGameplayTag Socket, AActor* HomingTarget, bool bOverridePitch = false, float PitchOverride = 0.0f);
};
