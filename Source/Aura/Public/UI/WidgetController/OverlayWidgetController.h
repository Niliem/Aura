// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;
class UAbilityInfo;
struct FOnAttributeChangeData;
struct FAuraAblityInfo;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag MessageTag = FGameplayTag();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Message = FText();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UAuraUserWidget> MessageWidget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowDelegate, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoDelegate, const FAuraAbilityInfo&, Info);

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbacksToDependencies() override;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedDelegate OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedDelegate OnMaxHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedDelegate OnManaChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedDelegate OnMaxManaChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FMessageWidgetRowDelegate MessageWidgetRowDelegate;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FAbilityInfoDelegate AbilityInfoDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Character")
    FOnAttributeChangedDelegate OnXPPercentChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
    TObjectPtr<UDataTable> MessageWidgetDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
    TObjectPtr<UAbilityInfo> AbilityInfo;

    void OnInitializedStartupAbilities();

    template <typename T>
    T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const;
};

template <typename T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const
{
    return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
