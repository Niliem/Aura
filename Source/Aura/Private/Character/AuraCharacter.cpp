// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Init ability actor info for the Server
    InitAbilityActorInfo();
    InitializeDefaultAttributes();
    AddStartupAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    // Init ability actor info for the Client
    InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCharacterLevel() const
{
    auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    check(AuraPlayerState);
    return AuraPlayerState->GetCharacterLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
    auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    check(AuraPlayerState);

    AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
    Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
    AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
    AttributeSet = AuraPlayerState->GetAttributeSet();

    if (auto AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
    {
        if (auto AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
        {
            AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, GetAbilitySystemComponent(), GetAttributeSet());
        }
    }
}

void AAuraCharacter::InitializeDefaultAttributes()
{
    ApplyGameplayEffectToSelf(DefaultPrimaryAttributes, 1.0f);
    ApplyGameplayEffectToSelf(DefaultSecondaryAttributes, 1.0f);
    ApplyGameplayEffectToSelf(DefaultVitalAttributes, 1.0f);
}
