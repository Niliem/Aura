// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAblityInfoForTag(const FGameplayTag& Tag) const
{
    for (auto& AbilityInfo : AbilityInformation)
    {
        if (AbilityInfo.AbilityTag.MatchesTagExact(Tag))
        {
            return AbilityInfo;
        }
    }

    UE_LOG(LogAura, Error, TEXT("Can't find Info for AblityTag [%s] on AbilityInfo [%s]."), *Tag.ToString(), *GetNameSafe(this));

    return FAuraAbilityInfo();
}
