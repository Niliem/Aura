// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UAbilitySystemComponent;

struct FWidgetControllerParams;

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
    static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
    static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
    static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
    static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay")
    static AAuraGameModeBase* GetAuraGameMode(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay")
    static TArray<AActor*> GetLiveActorsWithinRadius(const UObject* WorldContextObject, TSubclassOf<AActor> RequiredActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay")
    static bool IsOnSameTeam(const AActor* FirstActor, const AActor* SecondActor);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay")
    static int32 GetStencilValueFromEnum(const EStencilValue StencilValue);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageTypes")
    static TMap<FGameplayTag, FGameplayTag> GetDamageTypesToResistances(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static void ExecuteActivePeriodicEffectsWithTags(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer Tags);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
    static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);
};
