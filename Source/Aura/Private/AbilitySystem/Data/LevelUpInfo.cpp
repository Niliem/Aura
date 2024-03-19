// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/LevelUpInfo.h"

int ULevelUpInfo::FindLevelForXP(int32 XP) const
{
    for (int32 i = 0; i < LevelUpInformation.Num(); ++i)
    {
        if (LevelUpInformation[i].LevelUpRequirement > XP)
        {
            return i;
        }
    }
    return LevelUpInformation.Num();
}
