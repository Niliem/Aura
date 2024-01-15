// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
    GENERATED_BODY()

    FEffectProperties()
    {
    }

    FGameplayEffectContextHandle GameplayEffectContextHandle;

    UPROPERTY()
    UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;

    UPROPERTY()
    AActor* SourceAvatarActor = nullptr;

    UPROPERTY()
    AController* SourceController = nullptr;

    UPROPERTY()
    ACharacter* SourceCharacter = nullptr;

    UPROPERTY()
    UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;

    UPROPERTY()
    AActor* TargetAvatarActor = nullptr;

    UPROPERTY()
    AController* TargetController = nullptr;

    UPROPERTY()
    ACharacter* TargetCharacter = nullptr;
};

/**
 *
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UAuraAttributeSet();

    // Primary attributes
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

    // Secondary attributes
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

    // Vital attributes
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    UFUNCTION()
    void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

    UFUNCTION()
    void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

    UFUNCTION()
    void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

    UFUNCTION()
    void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

    UFUNCTION()
    void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

    UFUNCTION()
    void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

    UFUNCTION()
    void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

    UFUNCTION()
    void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

    UFUNCTION()
    void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

    UFUNCTION()
    void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

    UFUNCTION()
    void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

    UFUNCTION()
    void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

private:
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Strength;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Intelligence;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Resilience;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Vigor;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Armor;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData ArmorPenetration;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData BlockChance;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData CriticalHitChance;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData CriticalHitDamage;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData CriticalHitResistance;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData HealthRegeneration;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData ManaRegeneration;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxHealth;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxMana;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Health;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Mana;

    void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties);
};
