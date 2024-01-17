// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AuraProjectileAbility.generated.h"

class AAuraProjectileActor;

/**
 *
 */
UCLASS()
class AURA_API UAuraProjectileAbility : public UAuraGameplayAbility
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AAuraProjectileActor> ProjectileActorClass;

    UFUNCTION(BlueprintCallable)
    void SpawnProjectile(const FVector& TargetLocation);
};
