// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Data/AttributeInfo.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"

EDataValidationResult FAuraAttributeInfo::IsDataValid(FDataValidationContext& Context, const int Index) const
{
    EDataValidationResult Result = EDataValidationResult::Valid;

    if (!Attribute.IsValid())
    {
        Result = EDataValidationResult::Invalid;
        Context.AddError(FText::FromString(FString::Printf(TEXT("Attribute at index [%i] is empty!"), Index)));
    }
    if (!Tag.IsValid())
    {
        Result = EDataValidationResult::Invalid;
        Context.AddError(FText::FromString(FString::Printf(TEXT("Tag at index [%i] is empty!"), Index)));
    }

    return Result;
}

EDataValidationResult UAttributeInfo::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

    unsigned int Index = 0;
    for (auto& AttributeInfo : AttributeInformation)
    {
        Result = CombineDataValidationResults(Result, AttributeInfo.IsDataValid(Context, Index));
        ++Index;
    }

    return Result;
}
#endif

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
