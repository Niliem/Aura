// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& Tag) const
{
    for (auto& AttributeInfo : AttributeInformation)
    {
        if (AttributeInfo.Tag.MatchesTagExact(Tag))
        {
            return AttributeInfo;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *Tag.ToString(), *GetNameSafe(this));

    return FAuraAttributeInfo();
}
