// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Base.generated.h"

USTRUCT(BlueprintType)
struct FAttributeCoefficient
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayAttribute Attribute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Coefficient = 1.0f;
};

UCLASS()
class AURA_API UMMC_Base : public UGameplayModMagnitudeCalculation
{
    GENERATED_BODY()

public:
    virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
    UPROPERTY(EditAnywhere)
    float BaseValue = 0.0f;

    UPROPERTY(EditAnywhere)
    TArray<FAttributeCoefficient> AttributeCoefficients;

    UPROPERTY(EditAnywhere)
    float LevelCoefficient = 0.0f;
};
