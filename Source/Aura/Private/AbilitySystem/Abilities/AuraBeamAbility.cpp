// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraBeamAbility.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UAuraBeamAbility::TraceFirstTarget(const FVector& BeamStartLocation, const FVector& BeamTargetLocation)
{
    if (!IsValid(OwnerCharacter))
        return;

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerCharacter);
    FHitResult HitResult;
    UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter, BeamStartLocation, BeamTargetLocation, 10.0f, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
    if (HitResult.bBlockingHit)
    {
        CursorHitLocation = HitResult.ImpactPoint;
        CursorHitTarget = HitResult.GetActor();
    }
}
