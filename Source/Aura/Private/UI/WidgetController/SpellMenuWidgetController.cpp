// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
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

            ProcessAbilitySelection(GetSelectedAbilityStatusTag(), NewValue);
        });

    GetAuraAbilitySystemComponent()->OnAbilityStatusChanged.AddLambda(
        [this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 Level)
        {
            if (AbilityInfo)
            {
                FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
                Info.StatusTag = StatusTag;
                AbilityInfoDelegate.Broadcast(Info);
            }

            if (SelectedAbilityTag.MatchesTagExact(AbilityTag))
            {
                ProcessAbilitySelection(StatusTag, GetAuraPlayerState()->GetSpellPoints());
            }
        });
    GetAuraAbilitySystemComponent()->OnAbilityGiven.AddLambda(
        [this](const FGameplayTag& AbilityTag)
        {
            if (SelectedAbilityTag.MatchesTagExact(AbilityTag))
            {
                ProcessAbilitySelection(GetSelectedAbilityStatusTag(), GetAuraPlayerState()->GetSpellPoints());
            }
        });
    GetAuraAbilitySystemComponent()->OnAbilityEquipped.AddLambda(
        [this](const FGameplayTag& AbilityTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag)
        {
            SelectAbility(AbilityTag);

            if (PrevInputTag.IsValid())
            {
                FAuraAbilityInfo PrevInputInfo;
                PrevInputInfo.AbilityTag = FGameplayTag();
                PrevInputInfo.InputTag = PrevInputTag;
                AbilityInfoDelegate.Broadcast(PrevInputInfo);
            }

            if (AbilityInfo)
            {
                FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
                Info.StatusTag = AuraGameplayTags::Ability_Status_Equipped;
                Info.InputTag = InputTag;
                AbilityInfoDelegate.Broadcast(Info);
            }
        });
}

void USpellMenuWidgetController::SelectAbility(const FGameplayTag& AbilityTag)
{
    SelectedAbilityTag = AbilityTag;
    StopWaitingForEquipDelegate.Broadcast(FGameplayTag());
    bWaitingForEquipSelection = false;

    ProcessAbilitySelection(GetSelectedAbilityStatusTag(), GetAuraPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputSlotTag)
{
    GetAuraAbilitySystemComponent()->EquipAbility(AbilityTag, InputSlotTag);
}

void USpellMenuWidgetController::SpendSpellPoint()
{
    GetAuraAbilitySystemComponent()->SpendSpellPoint(SelectedAbilityTag);
}

void USpellMenuWidgetController::EquipButtonPressed()
{
    WaitForEquipDelegate.Broadcast(GetSelectedAbilityTypeTag());
    bWaitingForEquipSelection = true;
}

void USpellMenuWidgetController::SelectInputSlot(const FGameplayTag& InputSlotTag, const FGameplayTag& AbilityType)
{
    if (!bWaitingForEquipSelection)
    {
        return;
    }

    FGameplayTag SelectedAbilityType = GetSelectedAbilityTypeTag();
    if (!SelectedAbilityType.MatchesTagExact(AbilityType))
    {
        return;
    }

    EquipAbility(SelectedAbilityTag, InputSlotTag);
}

void USpellMenuWidgetController::ProcessAbilitySelection(const FGameplayTag& StatusTag, int32 SpellPoints)
{
    bool bCanEquipAbility = false;
    bool bCanSpendPoints = false;

    if (StatusTag.IsValid())
    {
        if (StatusTag.MatchesTagExact(AuraGameplayTags::Ability_Status_Equipped) || StatusTag.MatchesTagExact(AuraGameplayTags::Ability_Status_Unlocked))
        {
            bCanEquipAbility = true;
        }

        // TODO::
        bool bUpgradable = (StatusTag.MatchesTagExact(AuraGameplayTags::Ability_Status_Equipped) || StatusTag.MatchesTagExact(AuraGameplayTags::Ability_Status_Unlocked));

        if (SpellPoints > 0)
        {
            if (StatusTag.MatchesTagExact(AuraGameplayTags::Ability_Status_Eligible) || bUpgradable)
            {
                bCanSpendPoints = true;
            }
        }
    }

    FString Description = FString();
    FString NextLevelDescription = FString();
    if (SelectedAbilityTag.IsValid())
    {
        GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbilityTag, Description, NextLevelDescription);
    }

    OnSelectAbility.Broadcast(bCanSpendPoints, bCanEquipAbility, Description, NextLevelDescription);
}

FGameplayTag USpellMenuWidgetController::GetSelectedAbilityTypeTag()
{
    FGameplayTag AbilityType = FGameplayTag();
    if (const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(SelectedAbilityTag))
    {
        AbilityType = UAuraAbilitySystemLibrary::GetAbilityTypeTagFromSpec(*AbilitySpec);
    }

    if (!AbilityType.IsValid() && AbilityInfo)
    {
        FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(SelectedAbilityTag);
        if (Info.Ability)
        {
            AbilityType = UAuraAbilitySystemLibrary::GetAbilityTypeTag(Info.Ability.GetDefaultObject());
        }
    }
    return AbilityType;
}

FGameplayTag USpellMenuWidgetController::GetSelectedAbilityStatusTag()
{
    if (SelectedAbilityTag.IsValid())
    {
        if (const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(SelectedAbilityTag))
        {
            return UAuraAbilitySystemLibrary::GetAbilityStatusTagFromSpec(*AbilitySpec);
        }
    }
    return FGameplayTag::EmptyTag;
}
