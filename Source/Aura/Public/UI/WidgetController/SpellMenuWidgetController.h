// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSelectAbility, bool, bCanSpenPoint, bool, bCanEquip, FString, Description, FString, NextLevelDescription);

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

    UFUNCTION(BlueprintCallable, Category= "Spell Menu")
    void SelectAbility(const FGameplayTag& AbilityTag);

    UFUNCTION(BlueprintCallable, Category= "Spell Menu")
    void SpendSpellPoint();

    UPROPERTY(BlueprintAssignable, Category = "Spell Menu")
    FOnStatChangedDelegate OnSpellPointsChanged;

    UPROPERTY(BlueprintAssignable, Category = "Spell Menu")
    FOnSelectAbility OnSelectAbility;

private:
    FGameplayTag SelectedAbilityTag;

    void ProcessAbilitySelection(const FGameplayTag& StatusTag, int32 SpellPoints);

    FGameplayTag GetSelectedAbilityStatusTag();
};
