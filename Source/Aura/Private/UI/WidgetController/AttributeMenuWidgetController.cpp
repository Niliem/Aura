// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
    if (auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
    {
        check(AttributeInfo);

        FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(TAG_Attributes_Primary_Strength);
        Info.Value = AuraAttributeSet->GetStrength();

        AttributeInfoDelegate.Broadcast(Info);
    }
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {}
