// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Summoning")
    TArray<FVector> GetSpawnLocations();

    UFUNCTION(BlueprintPure, Category = "Summoning")
    TSubclassOf<APawn> GetSpawnClass();

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    int32 NumOfMinions = 1;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    TArray<TSubclassOf<APawn>> MinionClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float MinSpawnDistance = 50.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float MaxSpawnDistance = 250.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float SpawnSpread = 90.0f;
};
