// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AttributeMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

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
    FAttributeInfoSignature AttributeInfoDelegate;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem|Attributes")
    TObjectPtr<UAttributeInfo> AttributeInfo;

    void BroadcastAttributeInfo(const FGameplayTag& Tag) const;
};
