// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
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

USTRUCT(BlueprintType)
struct FGameplaySet_Ability
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameplayAbility> GameplayAbility = nullptr;

    UPROPERTY(EditDefaultsOnly)
    int32 AbilityLevel = 1;
};

UCLASS(BlueprintType, Const)
class UAbilitySet : public UPrimaryDataAsset
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TArray<FGameplaySet_Ability> GameplayAbilitySets;

public:
    void GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const;
};


USTRUCT(BlueprintType)
struct FGameplaySet_Effect
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

    UPROPERTY(EditDefaultsOnly)
    float EffectLevel = 1.0f;
};

UCLASS(BlueprintType, Const)
class UEffectSet : public UPrimaryDataAsset
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TArray<FGameplaySet_Effect> GameplayEffectSets;

public:
    void GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const;
};

USTRUCT(BlueprintType)
struct FAbilityInputBinding
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AbilityTag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
    FGameplayTag InputTag = FGameplayTag();
};

UCLASS(BlueprintType, Const)
class UAbilityInputBindings : public UPrimaryDataAsset
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TArray<FAbilityInputBinding> AbilityInputBindings;

public:
    void BindAbilityInputs(UAbilitySystemComponent* AbilitySystemComponent) const;
};

UCLASS(BlueprintType, Const)
class AURA_API UCharacterGameplayInfo : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    void InitializeGameplayInfo(UAbilitySystemComponent* AbilitySystemComponent);
    int32 GetXPReward(int32 Level = 1) const;

    UPROPERTY(EditDefaultsOnly)
    ECharacterClass CharacterClass;

    UPROPERTY(EditDefaultsOnly)
    FScalableFloat XPReward;

    UPROPERTY(EditDefaultsOnly)
    TArray<TObjectPtr<UAbilitySet>> GrantedAbilitySets;

    UPROPERTY(EditDefaultsOnly)
    TArray<TObjectPtr<UEffectSet>> GrantedEffectSets;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAbilityInputBindings> AbilityInputBinding;
};
