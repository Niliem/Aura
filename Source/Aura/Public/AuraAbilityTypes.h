// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

public:
    bool IsBlockedHit() const
    {
        return bIsBlockedHit;
    }

    bool IsCriticalHit() const
    {
        return bIsCriticalHit;
    }

    void SetIsBlockedHit(bool bInIsBlockedHit)
    {
        bIsBlockedHit = bInIsBlockedHit;
    }

    void SetIsCriticalHit(bool bInIsCriticalHit)
    {
        bIsCriticalHit = bInIsCriticalHit;
    }

    virtual UScriptStruct* GetScriptStruct() const
    {
        return FGameplayEffectContext::StaticStruct();
    }

    virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:
    UPROPERTY()
    bool bIsBlockedHit = false;

    UPROPERTY()
    bool bIsCriticalHit = false;
};
