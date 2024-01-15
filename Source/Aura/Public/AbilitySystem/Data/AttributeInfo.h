// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag Tag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Name = FText();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Description = FText();

    UPROPERTY(BlueprintReadOnly)
    float Value = 0.0f;
};

/**
 *
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& Tag) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "Tag"))
    TArray<FAuraAttributeInfo> AttributeInformation;
};
