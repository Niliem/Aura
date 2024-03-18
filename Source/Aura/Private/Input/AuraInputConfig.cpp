// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/AuraInputConfig.h"
#include "Aura/AuraLogChannels.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
EDataValidationResult FAuraInputAction::IsDataValid(FDataValidationContext& Context, const int Index) const
{
    EDataValidationResult Result = EDataValidationResult::Valid;

    if (InputAction == nullptr)
    {
        Result = EDataValidationResult::Invalid;
        Context.AddError(FText::FromString(FString::Printf(TEXT("InputAction at index [%i] is empty!"), Index)));
    }
    if (!InputTag.IsValid())
    {
        Result = EDataValidationResult::Invalid;
        Context.AddError(FText::FromString(FString::Printf(TEXT("InputTag at index [%i] is empty!"), Index)));
    }

    return Result;
}

EDataValidationResult UAuraInputConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

    unsigned int Index = 0;
    for (auto& AuraInputAction : AbilityInputActions)
    {
        Result = CombineDataValidationResults(Result, AuraInputAction.IsDataValid(Context, Index));
        ++Index;
    }

    return Result;
}
#endif

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
    for (auto& AuraInputAction : AbilityInputActions)
    {
        if (AuraInputAction.InputTag.MatchesTagExact(InputTag))
        {
            return AuraInputAction.InputAction;
        }
    }

    UE_LOG(LogAura, Error, TEXT("Can't find InputAction for InputTag [%s] on AttributeInfo [%s]."), *InputTag.ToString(), *GetNameSafe(this));

    return nullptr;
}
