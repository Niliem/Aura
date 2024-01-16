// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (c++)"), true, true, FLinearColor::Yellow);
}
