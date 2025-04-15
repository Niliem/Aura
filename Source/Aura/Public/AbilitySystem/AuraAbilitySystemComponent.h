// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenDelegate);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag&, const FGameplayTag&, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityGiven, const FGameplayTag&);

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    void AbilityActorInfoSet();

    void AbilityInputTagPressed(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag& InputTag);
    void AbilityInputTagHeld(const FGameplayTag& InputTag);

    UFUNCTION(BlueprintCallable, Category = "GameplayEffects")
    void ExecuteActivePeriodicEffectsWithTags(const FGameplayTagContainer& Tags);

    FEffectAssetTagsDelegate EffectAssetTags;
    FAbilitiesGivenDelegate OnAbilitiesGiven;
    FAbilityStatusChanged OnAbilityStatusChanged;
    FOnAbilityGiven OnAbilityGiven;

    bool bStartupAbilitiesGiven = false;

    void ForEachAbility(const FForEachAbility& Delegate);

    FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

    bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

    void AssignAbilityToInputTag(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
    void AssignAbilityToInputTag(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& InputTag);
    void ClearInputTag(FGameplayAbilitySpec* AbilitySpec);

    void UpgradeAttribute(const FGameplayTag& AttributeEventTag);

    void SpendSpellPoint(const FGameplayTag& AbilityTag);

    void UpdateAbilityStatuses(const int32 Level = 1);
protected:
    UFUNCTION(Server, Reliable)
    void ServerUpgradeAttribute(const FGameplayTag& AttributeEventTag);

    UFUNCTION(Server, Reliable)
    void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

    UFUNCTION(Client, Reliable)
    void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayHandle);

    UFUNCTION(Client, Reliable)
    void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 Level);

    virtual void OnRep_ActivateAbilities() override;
    virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

private:
    void ExecuteActivePeriodicEffect(const FActiveGameplayEffectHandle Handle);
};

