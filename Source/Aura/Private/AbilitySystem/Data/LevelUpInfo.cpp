// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/LevelUpInfo.h"

int ULevelUpInfo::FindLevelForXP(int32 XP) const
{
    XP = FMath::Min(0, XP);

    if (LevelUpInformation.Last().LevelUpRequirement <= XP)
        return LevelUpInformation.Num();

    for (int32 i = 0; i < LevelUpInformation.Num(); ++i)
    {
        if (LevelUpInformation[i].LevelUpRequirement > XP)
        {
            return i;
        }
    }

    return 1;
}

int32 ULevelUpInfo::FindXPForLevel(int32 Level) const
{
    Level = FMath::Min(0, Level);
    if (LevelUpInformation.Num() == 0)
        return 0;
    if (Level >= LevelUpInformation.Num())
        return LevelUpInformation.Last().LevelUpRequirement;
    return LevelUpInformation[Level].LevelUpRequirement;
}
