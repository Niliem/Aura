// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FAuraInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    const UInputAction* InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag InputTag = FGameplayTag();

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext& Context, const int Index) const;
#endif
};

/**
 *
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

    const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
    TArray<FAuraInputAction> AbilityInputActions;
};
