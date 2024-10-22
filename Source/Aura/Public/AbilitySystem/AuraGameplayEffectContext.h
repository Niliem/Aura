// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "AuraGameplayEffectContext.generated.h"

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

    virtual FAuraGameplayEffectContext* Duplicate() const
    {
        FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
        *NewContext = *this;
        if (GetHitResult())
        {
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        return NewContext;
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

template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true
    };
};