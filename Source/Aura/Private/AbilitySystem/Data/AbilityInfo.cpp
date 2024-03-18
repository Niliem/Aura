// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"

FAuraAblityInfo UAbilityInfo::FindAblityInfoForTag(const FGameplayTag& Tag) const
{
    for (auto& AblityInfo : AblityInformation)
    {
        if (AblityInfo.AbilityTag.MatchesTagExact(Tag))
        {
            return AblityInfo;
        }
    }

    UE_LOG(LogAura, Error, TEXT("Can't find Info for AblityTag [%s] on AblityInfo [%s]."), *Tag.ToString(), *GetNameSafe(this));

    return FAuraAblityInfo();
}
