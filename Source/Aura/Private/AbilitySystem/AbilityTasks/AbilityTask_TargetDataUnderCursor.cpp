// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AbilityTasks/AbilityTask_TargetDataUnderCursor.h"

UAbilityTask_TargetDataUnderCursor* UAbilityTask_TargetDataUnderCursor::CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility)
{
    UAbilityTask_TargetDataUnderCursor* MyObj = NewAbilityTask<UAbilityTask_TargetDataUnderCursor>(OwningAbility);
    return MyObj;
}

void UAbilityTask_TargetDataUnderCursor::Activate()
{
    if (!Ability)
        return;

    if (const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo())
    {
        if (ActorInfo->PlayerController.IsValid())
        {
            FHitResult CursorHit;
            ActorInfo->PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
            if (CursorHit.bBlockingHit)
                ValidData.Broadcast(CursorHit.ImpactPoint);
        }
    }
}
