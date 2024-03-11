// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CharacterGameplayInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
    None,
    AuraHero,
    Caster,
    Warrior,
    Ranger
};

USTRUCT()
struct FAbilitySet
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameplayAbility> Ability = nullptr;

    UPROPERTY(EditDefaultsOnly)
    int32 AbilityLevel = 1;

    UPROPERTY(EditDefaultsOnly)
    bool bIsScalable = false;

    UPROPERTY(EditDefaultsOnly)
    bool bActivateOnGranted = false;

    UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
    FGameplayTag InputTag;
};

/**
 *
 */
UCLASS(BlueprintType, Const)
class AURA_API UCharacterGameplayInfo : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    void GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent, float Level = 1.0f) const;
    void GiveEffects(UAbilitySystemComponent* AbilitySystemComponent, float Level = 1.0f) const;

    UPROPERTY(EditDefaultsOnly)
    ECharacterClass CharacterClass;

    UPROPERTY(EditDefaultsOnly)
    TArray<FAbilitySet> GrantedAbilitySets;

    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<UGameplayEffect>> GrantedEffects;
};
