// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/ModMagnitudeCalculation/MMC_Base.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include "Interaction/CombatInterface.h"

float UMMC_Base::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    int32 CharacterLevel = 1;
    if (auto CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject()))
    {
        CharacterLevel = CombatInterface->GetCharacterLevel();
    }

    float CapturedValues = 0.0f;
    for (auto& AttributeCoefficient : AttributeCoefficients)
    {
        if (auto CapturedAttribute = RelevantAttributesToCapture.FindByPredicate([AttributeCoefficient](const FGameplayEffectAttributeCaptureDefinition& Item) { return Item.AttributeToCapture == AttributeCoefficient.Attribute; }))
        {
            float AttributeValue = 0;
            GetCapturedAttributeMagnitude(*CapturedAttribute, Spec, EvaluationParameters, AttributeValue);

            CapturedValues += AttributeValue * AttributeCoefficient.Coefficient;
        }
    }

    return BaseValue + CapturedValues + LevelCoefficient * CharacterLevel;
}
