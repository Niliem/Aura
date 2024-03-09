// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Executions/DamageExecution.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraGameplayEffectContext.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"

struct FDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);

    TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

    FDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);

        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Secondary_Armor, ArmorDef);
        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Secondary_BlockChance, BlockChanceDef);
        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Resistance_Physical, PhysicalResistanceDef);
        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Resistance_Fire, FireResistanceDef);
        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Resistance_Arcane, ArcaneResistanceDef);
        TagsToCaptureDefs.Add(AuraGameplayTags::Attribute_Resistance_Lightning, LightningResistanceDef);
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

    RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
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

    const auto AuraGameMode = UAuraAbilitySystemLibrary::GetAuraGameMode(SourceAvatar);

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = SourceTags;
    EvaluateParameters.TargetTags = TargetTags;

    // Damage
    float TotalDamage = 0;
    for (const auto& Pair : UAuraAbilitySystemLibrary::GetDamageTypesToResistances(SourceAvatar))
    {
        const FGameplayTag DamageTypeTag = Pair.Key;
        const FGameplayTag ResistanceTag = Pair.Value;

        checkf(DamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
        const FGameplayEffectAttributeCaptureDefinition CaptureDef = DamageStatics().TagsToCaptureDefs[ResistanceTag];

        float Damage = Spec.GetSetByCallerMagnitude(Pair.Key);

        float Resistance = 0.0f;
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
        Resistance = FMath::Clamp(Resistance, 0.0f, 100.0f);

        Damage *= (100.0f - Resistance)/100.0f;

        TotalDamage += Damage;
    }

    // Block
    float TargetBlockChance = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
    TargetBlockChance = FMath::Max(TargetBlockChance, 0.0f);

    const bool bIsBlocked = TargetBlockChance >= FMath::FRandRange(UE_SMALL_NUMBER, 100.0f);
    UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bIsBlocked);
    if (bIsBlocked)
    {
        TotalDamage *= 0.1f;
    }

    // Armor
    float TargetArmor = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
    TargetArmor = FMath::Max(TargetArmor, 0.0f);

    float SourceArmorPenetration = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
    SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.0f);

    const FRealCurve* ArmorPenetrationCurve = AuraGameMode->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
    const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceLevel);

    const FRealCurve* EffectiveArmorCurve = AuraGameMode->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
    const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetLevel);

    const float EffectiveArmor = TargetArmor * (100.0f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.0f;
    TotalDamage *= (100.0f - EffectiveArmor * EffectiveArmorCoefficient) / 100.0f;

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

    const FRealCurve* CriticalHitResistanceCurve = AuraGameMode->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
    const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetLevel);

    const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;

    const bool bIsCriticalHit = EffectiveCriticalHitChance >= FMath::FRandRange(UE_SMALL_NUMBER, 100.0f);
    UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);
    if (bIsCriticalHit)
    {
        TotalDamage = TotalDamage * 2.0f + SourceCriticalHitDamage;
    }

    // Output
    const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, TotalDamage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
