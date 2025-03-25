// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
    OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());

    OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
    OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
    GetAuraPlayerState()->OnXPChangedDelegate.AddLambda(
            [this](int32 XP)
            {
                const ULevelUpInfo* LevelUpInfo = GetAuraPlayerState()->LevelUpInfo;
                checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

                const int32 Level = LevelUpInfo->GetLevelForXP(XP);
                const int32 PrevLevelXP = LevelUpInfo->GetXPForLevel(Level - 1);
                const int32 NextLevelXP = LevelUpInfo->GetXPForLevel(Level);

                if (PrevLevelXP == NextLevelXP)
                {
                    OnXPPercentChanged.Broadcast(1.0f);
                }
                else
                {
                    OnXPPercentChanged.Broadcast(static_cast<float>(XP - PrevLevelXP) / static_cast<float>(NextLevelXP - PrevLevelXP));
                }
            });
    GetAuraPlayerState()->OnLevelChangedDelegate.AddLambda(
        [this](int32 Level)
        {
            OnLevelChanged.Broadcast(Level);
        });

    if (GetAuraAbilitySystemComponent()->bStartupAbilitiesGiven)
    {
        BroadcastAbilityInfo();
    }
    else
    {
        GetAuraAbilitySystemComponent()->OnAbilitiesGiven.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
    }

    GetAuraAbilitySystemComponent()->EffectAssetTags.AddLambda(
        [this](const FGameplayTagContainer& AssetTags)
        {
            for (auto& Tag : AssetTags)
            {
                FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
                if (Tag.MatchesTag(MessageTag))
                {
                    if (auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
                        MessageWidgetRowDelegate.Broadcast(*Row);
                }
            }
        });

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute())
            .AddLambda(
                [this](const FOnAttributeChangeData& Data)
                {
                    OnHealthChanged.Broadcast(Data.NewValue);
                });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute())
        .AddLambda(
            [this](const FOnAttributeChangeData& Data)
            {
                OnMaxHealthChanged.Broadcast(Data.NewValue);
            });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute())
        .AddLambda(
            [this](const FOnAttributeChangeData& Data)
            {
                OnManaChanged.Broadcast(Data.NewValue);
            });
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute())
        .AddLambda(
            [this](const FOnAttributeChangeData& Data)
            {
                OnMaxManaChanged.Broadcast(Data.NewValue);
            });
}
