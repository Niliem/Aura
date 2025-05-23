// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraGameplayEffectContext.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    Super::NetSerialize(Ar, Map, bOutSuccess);

    uint32 RepBits = 7;
    if(Ar.IsSaving())
    {
        if(bIsBlockedHit)
        {
            RepBits |= 1 << 7;
        }
        if (bIsCriticalHit)
        {
            RepBits |= 1 << 8;
        }
        if (bIsSuccessfulDebuff)
        {
            RepBits |= 1 << 9;
        }
        if (DebuffDamage > 0.0f)
        {
            RepBits |= 1 << 10;
        }
        if (DebuffDuration > 0.0f)
        {
            RepBits |= 1 << 11;
        }
        if (DebuffFrequency > 0.0f)
        {
            RepBits |= 1 << 12;
        }
        if (DamageType.IsValid())
        {
            RepBits |= 1 << 13;
        }
        if (!DeathImpulse.IsZero())
        {
            RepBits |= 1 << 14;
        }
        if (!KnockbackImpulse.IsZero())
        {
            RepBits |= 1 << 15;
        }
    }

    Ar.SerializeBits(&RepBits, 16);

    if(RepBits & (1 << 7))
    {
        Ar << bIsBlockedHit;
    }
    if (RepBits & (1 << 8))
    {
        Ar << bIsCriticalHit;
    }
    if (RepBits & (1 << 9))
    {
        Ar << bIsSuccessfulDebuff;
    }
    if (RepBits & (1 << 10))
    {
        Ar << DebuffDamage;
    }
    if (RepBits & (1 << 11))
    {
        Ar << DebuffDuration;
    }
    if (RepBits & (1 << 12))
    {
        Ar << DebuffFrequency;
    }
    if (RepBits & (1 << 13))
    {
        if (Ar.IsLoading())
        {
            if (!DamageType.IsValid())
            {
                DamageType = MakeShared<FGameplayTag>();
            }
        }
        DamageType->NetSerialize(Ar, Map, bOutSuccess);
    }
    if (RepBits & (1 << 14))
    {
        DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
    }
    if (RepBits & (1 << 15))
    {
        KnockbackImpulse.NetSerialize(Ar, Map, bOutSuccess);
    }

    bOutSuccess = true;
    return true;
}
