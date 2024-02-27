// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecution.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UDamageExecution : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    UDamageExecution();

protected:
    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
