// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/LevelUpInfo.h"

int ULevelUpInfo::FindLevelForXP(int32 XP) const
{
    XP = FMath::Min(0, XP);

    if (LevelUpInformation.Last().LevelUpRequirement <= XP)
        return LevelUpInformation.Num();

    int32 Left = 0;
    int32 Right = LevelUpInformation.Num() - 1;
    int32 Mid = 0;
    while (Left <= Right)
    {
        Mid = Left + (Right - Left) / 2;
        if (LevelUpInformation[Mid - 1].LevelUpRequirement <= XP && LevelUpInformation[Mid].LevelUpRequirement > XP)
        {
            return Mid;
        }
        else if (LevelUpInformation[Mid].LevelUpRequirement > XP)
        {
            Right = Mid - 1;
        }
        else
        {
            Left = Mid + 1;
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
