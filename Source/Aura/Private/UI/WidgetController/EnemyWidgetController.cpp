// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/EnemyWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UEnemyWidgetController::BroadcastInitialValues()
{
    OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
    OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
}

void UEnemyWidgetController::BindCallbacksToDependencies()
{
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnHealthChanged.Broadcast(Data.NewValue);
        });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
        {
            OnMaxHealthChanged.Broadcast(Data.NewValue);
        });
}
