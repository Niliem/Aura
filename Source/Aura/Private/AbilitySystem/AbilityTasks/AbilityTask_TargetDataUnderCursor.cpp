// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AbilityTasks/AbilityTask_TargetDataUnderCursor.h"
#include "AbilitySystemComponent.h"

UAbilityTask_TargetDataUnderCursor* UAbilityTask_TargetDataUnderCursor::CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility)
{
    UAbilityTask_TargetDataUnderCursor* MyObj = NewAbilityTask<UAbilityTask_TargetDataUnderCursor>(OwningAbility);
    return MyObj;
}

void UAbilityTask_TargetDataUnderCursor::Activate()
{
    if (!Ability)
        return;

    const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
    if (bIsLocallyControlled)
    {
        SendTargetDataUnderCursor();
    }
    else
    {
    }
}

void UAbilityTask_TargetDataUnderCursor::SendTargetDataUnderCursor()
{
    FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

    FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

    if (const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo())
    {
        if (ActorInfo->PlayerController.IsValid())
        {
            FHitResult CursorHit;
            ActorInfo->PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
            Data->HitResult = CursorHit;
        }
    }

    FGameplayAbilityTargetDataHandle DataHandle;
    DataHandle.Add(Data);

    AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

    if (ShouldBroadcastAbilityTaskDelegates())
        ValidData.Broadcast(DataHandle);
}
