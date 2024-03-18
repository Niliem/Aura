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
    }

    SetReadyToDestroy();
    MarkAsGarbage();
}

void UWaitEffectTimeRemainingChange::Tick(float DeltaTime)
{
    Update.Broadcast(FMath::Max(GetEffectTimeRemaining(InEffectTag), 0.0f));
}

bool UWaitEffectTimeRemainingChange::IsTickable() const
{
    return bShouldTick;
}

TStatId UWaitEffectTimeRemainingChange::GetStatId() const
{
    return TStatId();
}

void UWaitEffectTimeRemainingChange::EffectTagChanged(const FGameplayTag EffectTag, int32 NewCount)
{
    if (NewCount > 0)
    {
        Start.Broadcast(FMath::Max(GetEffectTimeRemaining(EffectTag), 0.0f));
        bShouldTick = true;
    }
    if (NewCount == 0)
    {
        End.Broadcast(0.0f);
        bShouldTick = false;
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
    float MaxTimeRemaning = 0.0f;
    for (const auto TimeRemaning : TimesRemaining)
    {
        if (TimeRemaning > MaxTimeRemaning)
            MaxTimeRemaning = TimeRemaning;
    }

    return MaxTimeRemaning;
}
