// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

    NetUpdateFrequency = 100.0f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAuraPlayerState, Level);
    DOREPLIFETIME(AAuraPlayerState, XP);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
    return AttributeSet;
}

int32 AAuraPlayerState::GetLevel() const
{
    return Level;
}

void AAuraPlayerState::SetLevel(int32 NewLevel)
{
    Level = NewLevel;
    OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddToLevel(int32 ToLevel)
{
    Level += ToLevel;
    OnLevelChangedDelegate.Broadcast(Level);
}

int32 AAuraPlayerState::GetXP() const
{
    return XP;
}

void AAuraPlayerState::SetXP(int32 NewXP)
{
    XP = NewXP;
    TryLevelUp();
    OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToXP(int32 ToXP)
{
    XP += ToXP;
    TryLevelUp();
    OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::TryLevelUp()
{
    int32 NewLevel = LevelUpInfo->GetLevelForXP(XP);
    if (NewLevel > Level)
    {
        for (int i = Level + 1; i <= NewLevel; ++i)
        {
            LevelUpInfo->GetAttributePointRewardForLevel(i);
            LevelUpInfo->GetSpellPointRewardForLevel(i);
        }
        SetLevel(NewLevel);

        FGameplayEventData LevelUpPayload;
        LevelUpPayload.EventTag = OnLevelUpEvent;
        if (GetOwningController())
        {
            LevelUpPayload.Target = GetOwningController()->GetPawn();
            LevelUpPayload.Instigator = GetOwningController()->GetPawn();
        }
        LevelUpPayload.EventMagnitude = NewLevel;

        FScopedPredictionWindow NewScopedWindow(GetAbilitySystemComponent(), true);
        GetAbilitySystemComponent()->HandleGameplayEvent(LevelUpPayload.EventTag, &LevelUpPayload);
    }
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
    OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
    OnXPChangedDelegate.Broadcast(XP);
}
