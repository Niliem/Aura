// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraBeamAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
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

    if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(CursorHitTarget))
    {
        if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamAbility::PrimaryTargetDied))
        {
            CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamAbility::PrimaryTargetDied);
        }
    }
}

void UAuraBeamAbility::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerCharacter);
    ActorsToIgnore.Add(CursorHitTarget);

    TArray<AActor*> OutTargets = UAuraAbilitySystemLibrary::GetLiveActorsWithinRadius(GetAvatarActorFromActorInfo(), AActor::StaticClass(), ActorsToIgnore, 850.0f, CursorHitTarget->GetActorLocation());
    OutAdditionalTargets = UAuraAbilitySystemLibrary::GetClosestActors(OutTargets, GetNumShockTargets(GetAbilityLevel()), CursorHitTarget->GetActorLocation());

    for (const auto& Target : OutAdditionalTargets)
    {
        if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
        {
            if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamAbility::AdditionalTargetDied))
            {
                CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamAbility::AdditionalTargetDied);
            }
        }
    }
}

void UAuraBeamAbility::UnbindPrimaryTargetDied(AActor* DeadActor)
{
    if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(DeadActor))
    {
        if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamAbility::PrimaryTargetDied))
        {
            CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &UAuraBeamAbility::PrimaryTargetDied);
        }
    }
}

void UAuraBeamAbility::UnbindAdditionalTargetDied(AActor* DeadActor)
{
    if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(DeadActor))
    {
        if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamAbility::AdditionalTargetDied))
        {
            CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &UAuraBeamAbility::AdditionalTargetDied);
        }
    }
}

void UAuraBeamAbility::FormatAbilityDescription(int32 Level, FText& OutDescription)
{
    Super::FormatAbilityDescription(Level, OutDescription);

    OutDescription = FText::FormatNamed(OutDescription,
    "Target_Num",
    GetNumShockTargets(Level),
    "Target_Num_Next",
    GetNumShockTargets(Level + 1)
    );
}

int32 UAuraBeamAbility::GetNumShockTargets_Implementation(int32 Level) const
{
    return NumShockTargets;
}
