// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/EnemyWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UEnemyWidgetController::BroadcastInitialValues()
{
    if (auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
    {
        OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
        OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
    }
}

void UEnemyWidgetController::BindCallbacksToDependencies()
{
    if (auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
            .AddLambda(
                [this](const FOnAttributeChangeData& Data)
                {
                    OnHealthChanged.Broadcast(Data.NewValue);
                });
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
            .AddLambda(
                [this](const FOnAttributeChangeData& Data)
                {
                    OnMaxHealthChanged.Broadcast(Data.NewValue);
                });
    }
}
