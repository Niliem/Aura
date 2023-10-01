// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    if (auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
    {
        OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
        OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
    }
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    if (auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
    }
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
    OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
    OnMaxHealthChanged.Broadcast(Data.NewValue);
}
