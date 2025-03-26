// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
    GENERATED_BODY()

public:
    AAuraCharacter();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    // Inherited via ICombatInterface
    virtual int32 GetCharacterLevel() const override;

    // Inherited via IPlayerInterface
    virtual void AddToXP_Implementation(int32 InXP) override;
    virtual void AddToAttributePoints_Implementation(int InAttributePoints) override;
    virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;

private:
    virtual void InitAbilityActorInfo() override;
};
