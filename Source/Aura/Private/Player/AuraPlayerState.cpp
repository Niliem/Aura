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

    SetNetUpdateFrequency(100.0f);
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAuraPlayerState, Level);
    DOREPLIFETIME(AAuraPlayerState, XP);
    DOREPLIFETIME(AAuraPlayerState, AttributePoints);
    DOREPLIFETIME(AAuraPlayerState, SpellPoints);
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

void AAuraPlayerState::UpdateAbilityStatuses() const
{
    if (auto AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
    {
        AuraAbilitySystemComponent->UpdateAbilityStatuses(Level);
    }
}

void AAuraPlayerState::SetLevel(int32 NewLevel)
{
    Level = NewLevel;
    UpdateAbilityStatuses();
    OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddToLevel(int32 ToLevel)
{
    Level += ToLevel;
    UpdateAbilityStatuses();
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

int32 AAuraPlayerState::GetAttributePoints() const
{
    return AttributePoints;
}

void AAuraPlayerState::SetAttributePoints(int32 NewAttributePoints)
{
    AttributePoints = NewAttributePoints;
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToAttributePoints(int32 ToAttributePoints)
{
    AttributePoints += ToAttributePoints;
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

int32 AAuraPlayerState::GetSpellPoints() const
{
    return SpellPoints;
}

void AAuraPlayerState::SetSpellPoints(int32 NewSpellPoints)
{
    SpellPoints = NewSpellPoints;
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::AddToSpellPoints(int32 ToSpellPoints)
{
    SpellPoints += ToSpellPoints;
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::TryLevelUp()
{
    int32 NewLevel = LevelUpInfo->GetLevelForXP(XP);
    if (NewLevel > Level)
    {
        for (int i = Level; i < NewLevel; ++i)
        {
            AddToAttributePoints(LevelUpInfo->GetAttributePointRewardForLevel(i));
            AddToSpellPoints(LevelUpInfo->GetSpellPointRewardForLevel(i));
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
    UpdateAbilityStatuses();
    OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
    OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
