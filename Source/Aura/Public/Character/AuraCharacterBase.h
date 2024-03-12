// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UAttributeSet;
class UGameplayEffect;
class UCharacterGameplayInfo;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
    GENERATED_BODY()

public:
    AAuraCharacterBase();

    // Inherited via IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UAttributeSet* GetAttributeSet() const;

    // Inherited via ICombatInterface
    virtual UAnimMontage* GetHitReactMontage_Implementation() const override;
    virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) const override;
    virtual bool IsDead_Implementation() const override;
    virtual AActor* GetAvatar_Implementation() override;
    virtual void Die_Implementation() override;

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

    UPROPERTY(EditAnywhere, Category = "Combat")
    FName WeaponTipSocketName = FName("TipSocket");

    UPROPERTY(EditAnywhere, Category = "Combat")
    FName LeftHandSocketName = FName("LeftHandSocket");

    UPROPERTY(EditAnywhere, Category = "Combat")
    FName RightHandSocketName = FName("RightHandSocket");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
    TObjectPtr<UCharacterGameplayInfo> CharacterGameplayInfo = nullptr;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(Transient)
    TObjectPtr<UAttributeSet> AttributeSet;

    bool bIsDead = false;

    void Dissolve();

    UFUNCTION(BlueprintImplementableEvent)
    void AnimateMeshDissolveMaterial(UMaterialInstanceDynamic* DynamicMaterialInstance);

    UFUNCTION(BlueprintImplementableEvent)
    void AnimateWeaponDissolveMaterial(UMaterialInstanceDynamic* DynamicMaterialInstance);

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInstance> MeshDissolveMaterialInstance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    TObjectPtr<UAnimMontage> HitReactMontage;
};
