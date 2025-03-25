// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AsyncTasks/WaitEffectTimeRemainingChange.h"
#include "AbilitySystemComponent.h"

UWaitEffectTimeRemainingChange* UWaitEffectTimeRemainingChange::WaitForEffectTimeRemainingChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag EffectTag)
{
    UWaitEffectTimeRemainingChange* WaitEffectTimeRemainingChange = NewObject<UWaitEffectTimeRemainingChange>();
    WaitEffectTimeRemainingChange->AbilitySystemComponent = AbilitySystemComponent;
    WaitEffectTimeRemainingChange->InEffectTag = EffectTag;

    if (!IsValid(AbilitySystemComponent) || !EffectTag.IsValid())
    {
        WaitEffectTimeRemainingChange->EndTask();
        return nullptr;
    }

    AbilitySystemComponent->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitEffectTimeRemainingChange, &UWaitEffectTimeRemainingChange::EffectTagChanged);

    return WaitEffectTimeRemainingChange;
}

void UWaitEffectTimeRemainingChange::EndTask()
{
    if (IsValid(AbilitySystemComponent))
    {
        AbilitySystemComponent->RegisterGameplayTagEvent(InEffectTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
        AbilitySystemComponent->GetWorld()->GetTimerManager().ClearTimer(UpdateTimeRemainingTimer);
    }

    SetReadyToDestroy();
    MarkAsGarbage();
}

void UWaitEffectTimeRemainingChange::UpdateTimeRemaining()
{
    Update.Broadcast(FMath::Max(GetEffectTimeRemaining(InEffectTag), 0.0f));
}

void UWaitEffectTimeRemainingChange::EffectTagChanged(const FGameplayTag EffectTag, int32 NewCount)
{
    if (NewCount > 0)
    {
        if (IsValid(AbilitySystemComponent))
        {
            Start.Broadcast(FMath::Max(GetEffectTimeRemaining(EffectTag), 0.0f));
            AbilitySystemComponent->GetWorld()->GetTimerManager().SetTimer(UpdateTimeRemainingTimer, this, &UWaitEffectTimeRemainingChange::UpdateTimeRemaining, 0.1, true);
        }
    }
    if (NewCount == 0)
    {
        if (IsValid(AbilitySystemComponent))
        {
            AbilitySystemComponent->GetWorld()->GetTimerManager().ClearTimer(UpdateTimeRemainingTimer);
            End.Broadcast(0.0f);
        }
    }
}

float UWaitEffectTimeRemainingChange::GetEffectTimeRemaining(const FGameplayTag& EffectTag) const
{
    if (!IsValid(AbilitySystemComponent))
        return 0.0f;

    TArray<float> TimesRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemaining(FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(InEffectTag.GetSingleTagContainer()));
    if (TimesRemaining.Num() == 0)
    {
        TimesRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemaining(FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InEffectTag.GetSingleTagContainer()));
    }
    float MaxTimeRemaining = 0.0f;
    for (const auto TimeRemaining : TimesRemaining)
    {
        if (TimeRemaining > MaxTimeRemaining)
            MaxTimeRemaining = TimeRemaining;
    }

    return MaxTimeRemaining;
}
