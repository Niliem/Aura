// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegate, const FAuraAttributeInfo&, Info);

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

    UPROPERTY(BlueprintAssignable, Category = "AbilitySystem|Attributes")
    FAttributeInfoDelegate AttributeInfoDelegate;
    
    UPROPERTY(BlueprintAssignable, Category = "Character")
    FOnStatChangedDelegate OnAttributePointsChanged;

    UFUNCTION(BlueprintCallable)
    void UpgradeAttribute(const FGameplayTag& AttributeEventTag);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem|Attributes")
    TObjectPtr<UAttributeInfo> AttributeInfo;

    void BroadcastAttributeInfo(const FGameplayTag& Tag) const;
};
