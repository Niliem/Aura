// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayAttribute Attribute = FGameplayAttribute();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag Tag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Name = FText();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Description = FText();

    UPROPERTY(BlueprintReadOnly)
    float Value = 0.0f;

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext& Context, const int Index) const;
#endif
};

/**
 *
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

    FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& Tag) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "Attribute"))
    TArray<FAuraAttributeInfo> AttributeInformation;
};
