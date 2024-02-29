// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystemComponent.h"

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

    for (const auto& AbilityClass : ClassInfo->GetCommonAbilities())
    {
        FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
    }
    for (const auto& AbilityClass : ClassInfo->GetAbilitiesForClass(CharacterClass))
    {
        FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
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
