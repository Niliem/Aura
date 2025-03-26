// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
    BroadcastAbilityInfo();

    OnSpellPointsChanged.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
    GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
        [this](int32 NewValue)
        {
            OnSpellPointsChanged.Broadcast(NewValue);
        });

    GetAuraAbilitySystemComponent()->OnAbilityStatusChanged.AddLambda(
        [this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
        {
            if (AbilityInfo)
            {
                FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
                Info.StatusTag = StatusTag;
                AbilityInfoDelegate.Broadcast(Info);
            }
        });
}
