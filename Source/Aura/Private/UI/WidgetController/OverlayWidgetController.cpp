// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "Aura/AuraLogChannels.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    if (auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
    {
        OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
        OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

        OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
        OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

        OnInitializedStartupAbilities();
    }
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{

    if (auto AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
    {
        AuraPlayerState->OnXPChangedDelegate.AddLambda(
            [this, AuraPlayerState](int32 XP)
            {
                const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
                checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

                const int32 Level = LevelUpInfo->FindLevelForXP(XP);
                const int32 PrevLevelXP = LevelUpInfo->FindXPForLevel(Level - 1);
                const int32 NextLevelXP = LevelUpInfo->FindXPForLevel(Level);

                if (PrevLevelXP == NextLevelXP)
                {
                    OnXPPercentChanged.Broadcast(1.0f);
                }
                else
                {
                    OnXPPercentChanged.Broadcast(static_cast<float>(XP - PrevLevelXP) / static_cast<float>(NextLevelXP - PrevLevelXP));
                }
            });
    }

    if (auto AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        if (AuraAbilitySystemComponent->bStartupAbilitiesGiven)
        {
            OnInitializedStartupAbilities();
        }
        else
        {
            AuraAbilitySystemComponent->OnAbilitiesGiven.AddUObject(this, &UOverlayWidgetController::OnInitializedStartupAbilities);
        }

        AuraAbilitySystemComponent->EffectAssetTags.AddLambda(
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
    }

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
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
            .AddLambda(
                [this](const FOnAttributeChangeData& Data)
                {
                    OnManaChanged.Broadcast(Data.NewValue);
                });
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
            .AddLambda(
                [this](const FOnAttributeChangeData& Data)
                {
                    OnMaxManaChanged.Broadcast(Data.NewValue);
                });
    }
}

void UOverlayWidgetController::OnInitializedStartupAbilities()
{
    if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven)
            return;

        FForEachAbility BroadcastDelegate;
        BroadcastDelegate.BindLambda(
            [this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
            {
                FAuraAbilityInfo Info = AbilityInfo->FindAblityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
                Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
                if (const FGameplayTagContainer* Tags = AbilitySpec.Ability.Get()->GetCooldownTags())
                {
                    for (const auto& CooldownTag : Tags->GetGameplayTagArray())
                    {
                        Info.CooldownTag = CooldownTag;
                    }
                }
                AbilityInfoDelegate.Broadcast(Info);
            });
        AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
    }
}