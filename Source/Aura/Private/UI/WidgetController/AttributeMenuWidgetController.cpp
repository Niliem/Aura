// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
    check(AttributeInfo);
    for (auto& Info : AttributeInfo.Get()->AttributeInformation)
    {
        BroadcastAttributeInfo(Info.Tag);
    }

    if (auto AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
    {
        OnAttributePointsChanged.Broadcast(AuraPlayerState->GetAttributePoints());
    }
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
    if (auto AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
    {
        AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda(
            [this](int32 NewValue)
            {
                OnAttributePointsChanged.Broadcast(NewValue);
            });
    }

    check(AttributeInfo);
    for (auto& Info : AttributeInfo.Get()->AttributeInformation)
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.Attribute)
            .AddLambda(
                [this, Info](const FOnAttributeChangeData& Data)
                {
                    BroadcastAttributeInfo(Info.Tag);
                });
    }
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeEventTag)
{
    FGameplayEventData AttributePayload;
    AttributePayload.EventTag = AttributeEventTag;
    AttributePayload.EventMagnitude = 1.0f;
    FScopedPredictionWindow AttributeWindow(AbilitySystemComponent, true);
    AbilitySystemComponent->HandleGameplayEvent(AttributePayload.EventTag, &AttributePayload);

    FGameplayEventData AttributePointsPayload;
    AttributePointsPayload.EventTag = AuraGameplayTags::GameplayEvent_AttributePoints;
    AttributePointsPayload.EventMagnitude = -1.0f;
    FScopedPredictionWindow AttributePointsWindow(AbilitySystemComponent, true);
    AbilitySystemComponent->HandleGameplayEvent(AttributePointsPayload.EventTag, &AttributePointsPayload);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag) const
{
    FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
    Info.Value = Info.Attribute.GetNumericValue(AttributeSet);
    AttributeInfoDelegate.Broadcast(Info);
}
