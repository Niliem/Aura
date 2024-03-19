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

int32 AAuraPlayerState::GetCharacterLevel() const
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
    OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToXP(int32 ToXP)
{
    XP += ToXP;
    OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
    OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
    OnXPChangedDelegate.Broadcast(XP);
}
