// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedDelegate, int32);

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AAuraPlayerState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Inherited via IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UAttributeSet* GetAttributeSet() const;

    FOnPlayerStatChangedDelegate OnXPChangedDelegate;
    FOnPlayerStatChangedDelegate OnLevelChangedDelegate;
    FOnPlayerStatChangedDelegate OnAttributePointsChangedDelegate;
    FOnPlayerStatChangedDelegate OnSpellPointsChangedDelegate;

    int32 GetLevel() const;
    void SetLevel(int32 NewLevel);
    void AddToLevel(int32 ToLevel);

    int32 GetXP() const;
    void SetXP(int32 NewXP);
    void AddToXP(int32 ToXP);

    int32 GetAttributePoints() const;
    void SetAttributePoints(int32 NewAttributePoints);
    void AddToAttributePoints(int32 ToAttributePoints);

    int32 GetSpellPoints() const;
    void SetSpellPoints(int32 NewSpellPoints);
    void AddToSpellPoints(int32 ToSpellPoints);

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<ULevelUpInfo> LevelUpInfo;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag OnLevelUpEvent;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(Transient)
    TObjectPtr<UAttributeSet> AttributeSet;

    void TryLevelUp();

private:
    void UpdateAbilityStatuses() const;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
    int32 XP = 0;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
    int32 AttributePoints = 0;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
    int32 SpellPoints = 0;

    UFUNCTION()
    void OnRep_Level(int32 OldLevel);

    UFUNCTION()
    void OnRep_XP(int32 OldXP);

    UFUNCTION()
    void OnRep_AttributePoints(int32 OldAttributePoints);

    UFUNCTION()
    void OnRep_SpellPoints(int32 OldSpellPoints);
};
