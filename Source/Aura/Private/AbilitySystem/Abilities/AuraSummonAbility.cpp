// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
    if (NumOfMinions <= 0)
        return TArray<FVector>();

    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
    const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
    const float DeltaSpread = SpawnSpread / NumOfMinions;

    const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.0f, FVector::UpVector);
    UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 4.0f, FLinearColor::Blue, 2.0f);

    DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 8.0f, 12, FColor::Blue, false, 2.0f);
    DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 8.0f, 12, FColor::Blue, false, 2.0f);

    const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.0f, FVector::UpVector);
    UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.0f, FLinearColor::Blue, 2.0f);

    DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MinSpawnDistance, 8.0f, 12, FColor::Blue, false, 2.0f);
    DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 8.0f, 12, FColor::Blue, false, 2.0f);

    TArray<FVector> SpawnLocations;
    for (int32 i = 0; i < NumOfMinions; ++i)
    {
        const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i + DeltaSpread / 2.0f, FVector::UpVector);
        const FVector SpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
        SpawnLocations.Add(SpawnLocation);
        DrawDebugSphere(GetWorld(), SpawnLocation, 15.0f, 12, FColor::Purple, false, 2.0f);

        UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.0f, FLinearColor::Green, 2.0f);

        DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 8.0f, 12, FColor::Green, false, 2.0f);
        DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 8.0f, 12, FColor::Green, false, 2.0f);
    }

    return SpawnLocations;
}
