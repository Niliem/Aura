// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraGameplayEffectContext.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
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
