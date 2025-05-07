// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Game/AuraGameInstance.h"
#include "Player/AuraPlayerState.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraGameplayEffectContext.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Engine/OverlapResult.h"


bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        OutAuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
        if (OutAuraHUD)
        {
            if (AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>())
            {
                OutWCParams.PlayerController = PlayerController;
                OutWCParams.PlayerState = PlayerState;
                OutWCParams.AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
                OutWCParams.AttributeSet = PlayerState->GetAttributeSet();
                return true;
            }
        }
    }
    return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WidgetControllerParams;
    AAuraHUD* AuraHUD;
    if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
    {
        return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
    }

    return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WidgetControllerParams;
    AAuraHUD* AuraHUD;
    if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
    {
        return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
    }

    return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WidgetControllerParams;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
    {
        return AuraHUD->GetSpellMenuWidgetController(WidgetControllerParams);
    }

    return nullptr;
}

AAuraGameModeBase* UAuraAbilitySystemLibrary::GetAuraGameMode(const UObject* WorldContextObject)
{
    if (const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
        return AuraGameMode;
    return nullptr;
}

TArray<AActor*> UAuraAbilitySystemLibrary::GetLiveActorsWithinRadius(const UObject* WorldContextObject, TSubclassOf<AActor> RequiredActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
    TArray<AActor*> OutActors;

    FCollisionQueryParams SphereParams;
    SphereParams.AddIgnoredActors(ActorsToIgnore);

    if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        TArray<FOverlapResult> Overlaps;
        World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
        for (FOverlapResult& Overlap : Overlaps)
        {
            if (Overlap.GetActor() && Overlap.GetActor()->IsA(RequiredActors))
            {
                if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
                {
                    OutActors.AddUnique(Overlap.GetActor());
                }
            }
        }
    }

    return OutActors;
}

bool UAuraAbilitySystemLibrary::IsOnSameTeam(const AActor* FirstActor, const AActor* SecondActor)
{
    if (!IsValid(FirstActor) || !IsValid(SecondActor))
        return false;

    const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
    const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
    const bool bSameTeam = bBothArePlayers || bBothAreEnemies;
    return bSameTeam;
}

int32 UAuraAbilitySystemLibrary::GetStencilValueFromEnum(const EStencilValue StencilValue)
{
    return static_cast<int32>(StencilValue);
}

TMap<FGameplayTag, FGameplayTag> UAuraAbilitySystemLibrary::GetDamageTypesToResistances(const UObject* WorldContextObject)
{
    if (const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
        return AuraGameMode->DamageTypesToResistances;
    return TMap<FGameplayTag, FGameplayTag>();
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
    FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
    EffectContextHandle.AddSourceObject(DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor());

    const FGameplayEffectSpecHandle EffectSpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraGameplayTags::SetByCaller_Debuff_Chance, DamageEffectParams.DebuffChance);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraGameplayTags::SetByCaller_Debuff_Damage, DamageEffectParams.DebuffDamage);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraGameplayTags::SetByCaller_Debuff_Frequency, DamageEffectParams.DebuffFrequency);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, AuraGameplayTags::SetByCaller_Debuff_Duration, DamageEffectParams.DebuffDuration);

    DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

    return EffectContextHandle;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilitiesInfo(const UObject* WorldContextObject)
{
    if (const auto AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
        return AuraGameInstance->AbilityInfo;
    return nullptr;
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        return GetAbilityTag(AbilitySpec.Ability.Get());
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityTag(const UGameplayAbility* Ability)
{
    if (Ability)
    {
        for (auto Tag : Ability->GetAssetTags())
        {
            if (!Tag.MatchesTag(AuraGameplayTags::Ability_Status) && !Tag.MatchesTag(AuraGameplayTags::Ability_Type))
            {
                if (Tag.MatchesTag(AuraGameplayTags::Ability))
                {
                    return Tag;
                }
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (auto Tag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if (Tag.MatchesTag(AuraGameplayTags::Ability_Status))
        {
            return Tag;
        }
    }
    if (AbilitySpec.Ability)
    {
        return GetAbilityStatusTag(AbilitySpec.Ability.Get());
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityStatusTag(const UGameplayAbility* Ability)
{
    if (Ability)
    {
        for (auto Tag : Ability->GetAssetTags())
        {
            if (Tag.MatchesTag(AuraGameplayTags::Ability_Status))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityTypeTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (auto Tag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if (Tag.MatchesTag(AuraGameplayTags::Ability_Type))
        {
            return Tag;
        }
    }
    if (AbilitySpec.Ability)
    {
        return GetAbilityTypeTag(AbilitySpec.Ability.Get());
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityTypeTag(const UGameplayAbility* Ability)
{
    if (Ability)
    {
        for (auto Tag : Ability->GetAssetTags())
        {
            if (Tag.MatchesTag(AuraGameplayTags::Ability_Type))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (auto Tag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if (Tag.MatchesTag(AuraGameplayTags::InputTag))
        {
            return Tag;
        }
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityCooldownTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability)
    {
        return GetAbilityCooldownTag(AbilitySpec.Ability.Get());
    }
    return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemLibrary::GetAbilityCooldownTag(const UGameplayAbility* Ability)
{
    if (const FGameplayTagContainer* Tags = Ability->GetCooldownTags())
    {
        for (const auto& CooldownTag : Tags->GetGameplayTagArray())
        {
            return CooldownTag;
        }
    }
    return FGameplayTag();
}

FString UAuraAbilitySystemLibrary::GetAbilityDescription(const UObject* WorldContextObject, const FGameplayTag& AbilityTag, int32 Level, EAbilityDescriptionType DescriptionType)
{
    const auto Infos = GetAbilitiesInfo(WorldContextObject);
    if (!IsValid(Infos))
        return FString();

    FAuraAbilityInfo Info = GetAbilitiesInfo(WorldContextObject)->FindAbilityInfoForTag(AbilityTag);
    FText AbilityDescription = (DescriptionType == EAbilityDescriptionType::Upgrade) ? Info.UpgradeDescription : Info.Description;

    FString LevelDescription = (DescriptionType == EAbilityDescriptionType::Upgrade)
        ?
        FString::Printf(TEXT(
            "<Info>Level </><Gray>%i</><Info> > </><Level>%i</> \n"),
            Level,
            Level + 1
            )
        :
        FString::Printf(TEXT(
            "<Info>Level </><Level>%i</> \n"),
            Level
            );

    FString ManaCostDescription = FString();
    FString CooldownDescription = FString();

    if (UGameplayAbility* Ability = Info.Ability.GetDefaultObject())
    {
        FormatAbilityDescription(Ability, Level, AbilityDescription);

        if (const auto AuraAbility = Cast<UAuraGameplayAbility>(Ability))
        {
            float ManaCost = AuraAbility->GetManaCostAtLevel(Level);
            float NextManaCost = AuraAbility->GetManaCostAtLevel(Level + 1);
            if (DescriptionType == EAbilityDescriptionType::Upgrade && !FMath::IsNearlyEqual(ManaCost, NextManaCost))
            {
                ManaCostDescription = FString::Printf(TEXT(
                    "<Info>Mana - </><Gray>%.1f</><Info> > </><Mana>%.1f</> \n"),
                    ManaCost,
                    NextManaCost
                    );
            }
            else
            {
                ManaCostDescription = FString::Printf(TEXT(
                    "<Info>Mana - </><Mana>%.1f</> \n"),
                    ManaCost
                    );
            }

            float Cooldown = AuraAbility->GetCooldownAtLevel(Level);
            float NextCooldown = AuraAbility->GetCooldownAtLevel(Level + 1);
            if (DescriptionType == EAbilityDescriptionType::Upgrade && !FMath::IsNearlyEqual(Cooldown, NextCooldown))
            {
                CooldownDescription = FString::Printf(TEXT(
                    "<Info>Cooldown - </><Gray>%.1f</><Info> > </><Cooldown>%.1fs</> \n"),
                    Cooldown,
                    NextCooldown
                    );
            }
            else
            {
                CooldownDescription = FString::Printf(TEXT(
                    "<Info>Cooldown - </><Cooldown>%.1fs</> \n"),
                    Cooldown
                    );
            }
        }
    }

    FString OutDescription = FString::Printf(TEXT(
        "<Title>%s</> \n"
        "%s"
        "%s"
        "\n"
        "%s\n"),
        *Info.Name.ToString(),
        *LevelDescription,
        *(ManaCostDescription+CooldownDescription),
        *AbilityDescription.ToString()
        );

    if (DescriptionType == EAbilityDescriptionType::Require)
    {
        OutDescription += FString::Printf(TEXT(
            "\n"
            "<Small>Spell locked until level: %i</>"),
            Info.RequirementLevel);
    }

    return OutDescription;
}

void UAuraAbilitySystemLibrary::FormatAbilityDescription(UGameplayAbility* Ability, int32 Level, FText& OutDescription)
{
    if (UAuraDamageAbility* DamageAbility = Cast<UAuraDamageAbility>(Ability))
    {
        OutDescription = FText::FormatNamed(OutDescription,
            "Dmg_Fire",
            DamageAbility->GetDamageAtLevel(AuraGameplayTags::DamageType_Elemental_Fire, Level),
            "Dmg_Fire_Next",
            DamageAbility->GetDamageAtLevel(AuraGameplayTags::DamageType_Elemental_Fire, Level + 1)
            );
    }
}

void UAuraAbilitySystemLibrary::ExecuteActivePeriodicEffectsWithTags(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer Tags)
{
    if (const auto AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        AuraAbilitySystemComponent->ExecuteActivePeriodicEffectsWithTags(Tags);
    }
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if (const auto AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraContext->IsBlockedHit();
    }
    return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if (const auto AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraContext->IsCriticalHit();
    }
    return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit)
{
    if (auto AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraContext->SetIsBlockedHit(bIsBlockedHit);
    }
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit)
{
    if (auto AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraContext->SetIsCriticalHit(bIsCriticalHit);
    }
}
