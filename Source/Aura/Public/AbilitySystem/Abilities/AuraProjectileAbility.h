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
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AAuraProjectileActor> ProjectileActorClass;
};
