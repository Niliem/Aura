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

    int32 GetCharacterLevel() const;
    void SetLevel(int32 NewLevel);
    void AddToLevel(int32 ToLevel);

    int32 GetXP() const;
    void SetXP(int32 NewXP);
    void AddToXP(int32 ToXP);

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
    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
    int32 XP = 0;

    UFUNCTION()
    void OnRep_Level(int32 OldLevel);

    UFUNCTION()
    void OnRep_XP(int32 OldXP);
};
