// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AuraBeamAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraBeamAbility : public UAuraDamageAbility
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Beam")
    void StoreCursorDataInfo(const FHitResult& HitResult);

    UFUNCTION(BlueprintCallable, Category = "Beam")
    void StoreOwnerPlayerController();

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Beam")
    FVector CursorHitLocation;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Beam")
    TObjectPtr<AActor> CursorHitTarget;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Beam")
    TObjectPtr<APlayerController> OwnerPlayerController;
};
