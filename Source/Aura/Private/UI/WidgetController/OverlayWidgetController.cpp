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
