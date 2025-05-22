// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraBeamAbility.h"
#include "GameFramework/Character.h"

void UAuraBeamAbility::StoreCursorDataInfo(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        CursorHitLocation = HitResult.ImpactPoint;
        CursorHitTarget = HitResult.GetActor();
    }
    else
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}

void UAuraBeamAbility::StoreOwnerVariables()
{
    if (CurrentActorInfo)
    {
        OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
        OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
    }
}
