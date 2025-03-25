// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AbilityTag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<const UTexture2D> Icon = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag StatusTag = FGameplayTag();

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag TypeTag = FGameplayTag();

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag InputTag = FGameplayTag();

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag CooldownTag = FGameplayTag();
};

/**
 *
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& Tag) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "AbilityTag"))
    TArray<FAuraAbilityInfo> AbilityInformation;
};
