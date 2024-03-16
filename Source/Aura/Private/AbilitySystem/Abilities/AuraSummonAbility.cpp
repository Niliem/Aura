// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
    if (NumOfMinions <= 0)
        return TArray<FVector>();

    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
    const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
    const float DeltaSpread = SpawnSpread / NumOfMinions;
    const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.0f, FVector::UpVector);

    TArray<FVector> SpawnLocations;
    for (int32 i = 0; i < NumOfMinions; ++i)
    {
        const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i + DeltaSpread / 2.0f, FVector::UpVector);
        FVector SpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

        FHitResult Hit;
        GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation + FVector(0.0f, 0.0f, 400.0f), SpawnLocation - FVector(0.0f, 0.0f, 400.0f), ECC_Visibility);
        if (Hit.bBlockingHit)
        {
            SpawnLocation = Hit.ImpactPoint;
        }

        SpawnLocations.Add(SpawnLocation);
    }

    return SpawnLocations;
}
