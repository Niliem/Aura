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

    bool IsSuccessfulDebuff() const
    {
        return bIsSuccessfulDebuff;
    }

    float GetDebuffDamage() const
    {
        return DebuffDamage;
    }

    float GetDebuffDuration() const
    {
        return DebuffDuration;
    }

    float GetDebuffFrequency() const
    {
        return DebuffFrequency;
    }

    TSharedPtr<FGameplayTag> GetDamageType() const
    {
        return DamageType;
    }

    FVector GetDeathImpulse() const
    {
        return DeathImpulse;
    }

    void SetIsBlockedHit(bool bInIsBlockedHit)
    {
        bIsBlockedHit = bInIsBlockedHit;
    }

    void SetIsCriticalHit(bool bInIsCriticalHit)
    {
        bIsCriticalHit = bInIsCriticalHit;
    }

    void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff)
    {
        bIsSuccessfulDebuff = bInIsSuccessfulDebuff;
    }

    void SetDebuffDamage(float InDebuffDamage)
    {
        DebuffDamage = InDebuffDamage;
    }

    void SetDebuffDuration(float InDebuffDuration)
    {
        DebuffDuration = InDebuffDuration;
    }

    void SetDebuffFrequency(float InDebuffFrequency)
    {
        DebuffFrequency = InDebuffFrequency;
    }

    void SetDamageType(TSharedPtr<FGameplayTag> InDamageType)
    {
        DamageType = InDamageType;
    }

    void SetDeathImpulse(const FVector& InDeathImpulse)
    {
        DeathImpulse = InDeathImpulse;
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

    UPROPERTY()
    bool bIsSuccessfulDebuff = false;

    UPROPERTY()
    float DebuffDamage = 0.0f;

    UPROPERTY()
    float DebuffDuration = 0.0f;

    UPROPERTY()
    float DebuffFrequency = 0.0f;

    TSharedPtr<FGameplayTag> DamageType;

    UPROPERTY()
    FVector DeathImpulse = FVector::ZeroVector;
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
