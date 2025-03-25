// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

class AAuraPlayerController;
class AAuraPlayerState;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

class UAbilityInfo;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
    GENERATED_BODY()

    FWidgetControllerParams() {}

    FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
    : PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS)
    {
        //
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerController> PlayerController = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerState> PlayerState = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChangedDelegate, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoDelegate, const FAuraAbilityInfo&, Info);

/**
 *
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

    UFUNCTION(BlueprintCallable)
    virtual void BroadcastInitialValues();

    UFUNCTION(BlueprintCallable)
    virtual void BindCallbacksToDependencies();

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FAbilityInfoDelegate AbilityInfoDelegate;

    void BroadcastAbilityInfo();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
    TObjectPtr<UAbilityInfo> AbilityInfo;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerController> PlayerController;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerState> PlayerState;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAttributeSet> AttributeSet;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<AAuraPlayerController> AuraPlayerController;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<AAuraPlayerState> AuraPlayerState;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

    AAuraPlayerController* GetAuraPlayerController();
    AAuraPlayerState* GetAuraPlayerState();
    UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();
    UAuraAttributeSet* GetAuraAttributeSet();
};
