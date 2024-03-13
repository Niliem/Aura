// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
class UNiagaraSystem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class AURA_API ICombatInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual int32 GetCharacterLevel() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    FVector GetCombatSocketLocation(const FGameplayTag& SocketTag) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    UAnimMontage* GetHitReactMontage() const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
    void SetFacingTarget(const FVector& TargetLocation);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    bool IsDead() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    AActor* GetAvatar();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    void Die();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    UNiagaraSystem* GetBloodEffect();
};
