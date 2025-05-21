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
    virtual void FormatAbilityDescription(int32 Level, FText& OutDescription) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TSubclassOf<AAuraProjectileActor> ProjectileActorClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    float ProjectileSpread = 90.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    int32 NumProjectiles = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    bool bUseOverridePitch = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUseOverridePitch", EditConditionHides), Category = "Projectile")
    float OverridePitch = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    bool bLaunchHomingProjectiles = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    FFloatRange HomingAcceleration = FFloatRange(1600.0f, 3200.0f);

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void SpawnProjectiles(const FVector& TargetLocation, FGameplayTag Socket, AActor* HomingTarget = nullptr, bool bOverridePitch = false, float PitchOverride = 0.0f);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability", meta = (HidePin="Target"))
    int32 GetNumProjectiles(int32 Level) const;
};
