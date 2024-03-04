// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Executions/DamageExecution.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/AuraGameplayEffectContext.h"
#include "Interaction/CombatInterface.h"

struct FDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

    FDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
    }
};

static const FDamageStatics& DamageStatics()
{
    static FDamageStatics Statics;
    return Statics;
}

UDamageExecution::UDamageExecution()
{
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
    RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const UAbilitySystemComponent* SourceAbilityComponent = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetAbilityComponent = ExecutionParams.GetTargetAbilitySystemComponent();

    AActor* SourceAvatar = SourceAbilityComponent ? SourceAbilityComponent->GetAvatarActor() : nullptr;
    AActor* TargetAvatar = TargetAbilityComponent ? TargetAbilityComponent->GetAvatarActor() : nullptr;

    int32 SourceLevel = 1;
    if (TScriptInterface<ICombatInterface> SourceCombatInterface = SourceAvatar)
    {
        SourceLevel = SourceCombatInterface->GetCharacterLevel();
    }

    int32 TargetLevel = 1;
    if (TScriptInterface<ICombatInterface> TargetCombatInterface = TargetAvatar)
    {
        TargetLevel = TargetCombatInterface->GetCharacterLevel();
    }

    const UCharacterClassInfo* ClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = SourceTags;
    EvaluateParameters.TargetTags = TargetTags;

    // Damage
    float Damage = 0;
    FGameplayTagContainer DamageTypes = UGameplayTagsManager::Get().RequestGameplayTagChildren(AuraGameplayTags::DamageType);
    for (const auto& DamageType : DamageTypes)
    {
        Damage += Spec.GetSetByCallerMagnitude(DamageType);
    }

    // Block
    float TargetBlockChance = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
    TargetBlockChance = FMath::Max(TargetBlockChance, 0.0f);

    const bool bIsBlocked = TargetBlockChance >= FMath::FRandRange(UE_SMALL_NUMBER, 100.0f);
    UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bIsBlocked);
    if (bIsBlocked)
    {
        Damage *= 0.1f;
    }

    // Armor
    float TargetArmor = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
    TargetArmor = FMath::Max(TargetArmor, 0.0f);

    float SourceArmorPenetration = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
    SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.0f);

    const FRealCurve* ArmorPenetrationCurve = ClassInfo->GetDamageCalculationCoefficients()->FindCurve(FName("ArmorPenetration"), FString());
    const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceLevel);

    const FRealCurve* EffectiveArmorCurve = ClassInfo->GetDamageCalculationCoefficients()->FindCurve(FName("EffectiveArmor"), FString());
    const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetLevel);

    const float EffectiveArmor = TargetArmor * (100.0f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.0f;
    Damage *= (100.0f - EffectiveArmor * EffectiveArmorCoefficient) / 100.0f;

    // Critical chance
    float SourceCriticalHitChance = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
    SourceCriticalHitChance = FMath::Max(SourceCriticalHitChance, 0.0f);

    float SourceCriticalHitDamage = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
    SourceCriticalHitDamage = FMath::Max(SourceCriticalHitDamage, 0.0f);

    float TargetCriticalHitResistance = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
    TargetCriticalHitResistance = FMath::Max(TargetCriticalHitResistance, 0.0f);

    const FRealCurve* CriticalHitResistanceCurve = ClassInfo->GetDamageCalculationCoefficients()->FindCurve(FName("CriticalHitResistance"), FString());
    const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetLevel);

    const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;

    const bool bIsCriticalHit = EffectiveCriticalHitChance >= FMath::FRandRange(UE_SMALL_NUMBER, 100.0f);
    UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);
    if (bIsCriticalHit)
    {
        Damage = Damage * 2.0f + SourceCriticalHitDamage;
    }

    // Output
    const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
