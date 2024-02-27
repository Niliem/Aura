// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Executions/DamageExecution.h"
#include "AbilitySystemComponent.h"

UDamageExecution::UDamageExecution()
{
}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const UAbilitySystemComponent* SourceAbilityComponent = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetAbilityComponent = ExecutionParams.GetTargetAbilitySystemComponent();

    const AActor* SourceAvatar = SourceAbilityComponent ? SourceAbilityComponent->GetAvatarActor() : nullptr;
    const AActor* TargetAvatar = TargetAbilityComponent ? TargetAbilityComponent->GetAvatarActor() : nullptr;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
}
