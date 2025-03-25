// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
    PlayerController = WCParams.PlayerController;
    PlayerState = WCParams.PlayerState;
    AbilitySystemComponent = WCParams.AbilitySystemComponent;
    AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
    if (!GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
        return;

    FForEachAbility BroadcastDelegate;
    BroadcastDelegate.BindLambda(
        [this](const FGameplayAbilitySpec& AbilitySpec)
        {
            const FGameplayTag AbilityTag = GetAuraAbilitySystemComponent()->GetAbilityTagFromSpec(AbilitySpec);
            if (!AbilityTag.IsValid())
                return;

            FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
            Info.StatusTag = GetAuraAbilitySystemComponent()->GetAbilityStatusFromSpec(AbilitySpec);
            Info.TypeTag = GetAuraAbilitySystemComponent()->GetAbilityTypeFromSpec(AbilitySpec);
            Info.InputTag = GetAuraAbilitySystemComponent()->GetInputTagFromSpec(AbilitySpec);
            if (const FGameplayTagContainer* Tags = AbilitySpec.Ability.Get()->GetCooldownTags())
            {
                for (const auto& CooldownTag : Tags->GetGameplayTagArray())
                {
                    Info.CooldownTag = CooldownTag;
                }
            }
            AbilityInfoDelegate.Broadcast(Info);
        });
    GetAuraAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPlayerController()
{
    if (!IsValid(AuraPlayerController))
    {
        AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
    }
    return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPlayerState()
{
    if (!IsValid(AuraPlayerState))
    {
        AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
    }
    return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraAbilitySystemComponent()
{
    if (!IsValid(AuraAbilitySystemComponent))
    {
        AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
    }
    return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
    if (!IsValid(AuraAttributeSet))
    {
        AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
    }
    return AuraAttributeSet;
}
