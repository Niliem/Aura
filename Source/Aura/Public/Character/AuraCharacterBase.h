// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UAttributeSet;
class UGameplayEffect;
class UCharacterGameplayInfo;
class UNiagaraSystem;
struct FGameplayTag;
struct FGameplayTagContainer;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public ICombatInterface
{
    GENERATED_BODY()

public:
    AAuraCharacterBase();

    // Inherited via IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UAttributeSet* GetAttributeSet() const;

    // Inherited via IGameplayTagAssetInterface
    virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
    virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
    virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
    virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

    // Inherited via ICombatInterface
    virtual UAnimMontage* GetHitReactMontage_Implementation() const override;
    virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) const override;
    virtual bool IsDead_Implementation() const override;
    virtual void Die_Implementation() override;
    virtual int32 GetXPReward_Implementation() const override;
    virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
    virtual int32 GetMinionCount_Implementation() const override;
    virtual void UpdateMinionCount_Implementation(int32 Amount) override;

    UFUNCTION(NetMulticast, Reliable)
    virtual void MulticastHandleDeath();

protected:
    virtual void BeginPlay() override;

    virtual void InitAbilityActorInfo();
    virtual void InitializeDefaultAttributes();
    virtual void InitializeDefaultAbilities();

    void ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> EffectClassToApply, const float Level = 1.0f) const;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    FName WeaponHandSocketName = FName("WeaponHandSocket");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    FName WeaponTipSocketName = FName("TipSocket");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    FName LeftHandSocketName = FName("LeftHandSocket");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    FName RightHandSocketName = FName("RightHandSocket");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    FName TailSocketName = FName("TailSocket");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float BaseWalkSpeed = 600.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TObjectPtr<USoundBase> DeathSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TObjectPtr<UCharacterGameplayInfo> CharacterGameplayInfo = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    int32 MinionCount = 0;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(Transient)
    TObjectPtr<UAttributeSet> AttributeSet;

    void Dissolve();

    UFUNCTION(BlueprintImplementableEvent)
    void AnimateMeshDissolveMaterial(UMaterialInstanceDynamic* DynamicMaterialInstance);

    UFUNCTION(BlueprintImplementableEvent)
    void AnimateWeaponDissolveMaterial(UMaterialInstanceDynamic* DynamicMaterialInstance);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TObjectPtr<UMaterialInstance> MeshDissolveMaterialInstance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    UNiagaraSystem* BloodEffect;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    TObjectPtr<UAnimMontage> HitReactMontage;
};
