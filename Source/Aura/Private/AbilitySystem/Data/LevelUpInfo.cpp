// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/LevelUpInfo.h"

int ULevelUpInfo::GetLevelForXP(int32 XP) const
{
    XP = FMath::Max(0, XP);

    if (LevelUpInformation.Last().LevelUpRequirement <= XP)
        return LevelUpInformation.Num();

    for (int32 i = 0; i < LevelUpInformation.Num() - 1; ++i)
    {
        if (LevelUpInformation[i].LevelUpRequirement > XP)
        {
            return i;
        }
    }

    return 1;
}

int32 ULevelUpInfo::GetXPForLevel(int32 Level) const
{
    Level = FMath::Max(0, Level);
    if (LevelUpInformation.Num() == 0)
        return 0;
    if (Level >= LevelUpInformation.Num())
        return LevelUpInformation.Last().LevelUpRequirement;
    return LevelUpInformation[Level].LevelUpRequirement;
}

int32 ULevelUpInfo::GetAttributePointRewardForLevel(int32 Level) const
{
    if (LevelUpInformation.IsValidIndex(Level))
        return LevelUpInformation[Level].AttributePointReward;
    return 0;
}

int32 ULevelUpInfo::GetSpellPointRewardForLevel(int32 Level) const
{
    if (LevelUpInformation.IsValidIndex(Level))
        return LevelUpInformation[Level].SpellPointReward;
    return 0;
}
