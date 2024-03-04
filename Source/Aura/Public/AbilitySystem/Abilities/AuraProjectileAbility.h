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
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AAuraProjectileActor> ProjectileActorClass;

    UFUNCTION(BlueprintCallable)
    void SpawnProjectile(const FVector& TargetLocation);
};
