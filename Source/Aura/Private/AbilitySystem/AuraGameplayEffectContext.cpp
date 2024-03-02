// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraGameplayEffectContext.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    Super::NetSerialize(Ar, Map, bOutSuccess);

    uint8 RepBits = 0;
    if(Ar.IsSaving())
    {
        if(bIsBlockedHit)
        {
            RepBits |= 1 << 0;
        }
        if (bIsCriticalHit)
        {
            RepBits |= 1 << 1;
        }
    }

    Ar.SerializeBits(&RepBits, 2);

    if(RepBits & (1 << 0))
    {
        Ar << bIsBlockedHit;
    }
    if (RepBits & (1 << 1))
    {
        Ar << bIsCriticalHit;
    }

    bOutSuccess = true;
    return true;
}
