// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraGameplayEffectContext.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
        {
            if (AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>())
            {
                UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
                UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

                const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);

                return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
            }
        }
    }

    return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
        {
            if (AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>())
            {
                UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
                UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

                const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);

                return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
            }
        }
    }

    return nullptr;
}

void UAuraAbilitySystemLibrary::GiveAbilitiesForClass(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
    const auto ClassInfo = GetCharacterClassInfo(WorldContextObject);

    auto GiveAbilityWithActivation = [AbilitySystemComponent](TSubclassOf<UGameplayAbility> AbilityClass, bool bActivate)
    {
        FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        if (bActivate)
        {
            AbilitySystemComponent->GiveAbilityAndActivateOnce(GameplayAbilitySpec);
        }
        else
        {
            AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
        }
    };

    for (const auto& AbilityPair : ClassInfo->GetCommonAbilities())
    {
        GiveAbilityWithActivation(AbilityPair.Key, AbilityPair.Value);
    }
    for (const auto& AbilityPair : ClassInfo->GetAbilitiesForClass(CharacterClass))
    {
        GiveAbilityWithActivation(AbilityPair.Key, AbilityPair.Value);
    }
}

void UAuraAbilitySystemLibrary::InitializeAttributesForClass(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
    const auto ClassInfo = GetCharacterClassInfo(WorldContextObject);

    const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

    auto PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
    PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
    const auto PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassInfo->GetPrimaryAttributesForClass(CharacterClass), Level, PrimaryAttributesContextHandle);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

    auto SecondatyAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
    SecondatyAttributesContextHandle.AddSourceObject(AvatarActor);
    const auto SecondatyAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassInfo->GetSecondatyAttributesForClass(CharacterClass), Level, SecondatyAttributesContextHandle);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondatyAttributesSpecHandle.Data.Get());

    auto VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
    VitalAttributesContextHandle.AddSourceObject(AvatarActor);
    const auto VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassInfo->GetVitalAttributesForClass(CharacterClass), Level, VitalAttributesContextHandle);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
    const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (!IsValid(AuraGameMode))
        return nullptr;

    return AuraGameMode->CharacterClassInfo;
}

void UAuraAbilitySystemLibrary::ExecuteActivePeriodicEffectByTag(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag EffectTag)
{
    if (const auto AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        AuraAbilitySystemComponent->ExecuteActivePeriodicEffectByTag(EffectTag);
    }
}

TMap<FGameplayTag, FGameplayTag> UAuraAbilitySystemLibrary::GetDamageTypesToResistances(const UObject* WorldContextObject)
{
    const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (!IsValid(AuraGameMode))
        return TMap<FGameplayTag, FGameplayTag>();

    return AuraGameMode->DamageTypesToResistances;
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
