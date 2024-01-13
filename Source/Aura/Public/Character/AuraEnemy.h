// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
    GENERATED_BODY()

public:
    AAuraEnemy();

    // Inherited via IEnemyInterface
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;

    // Inherited via ICombatInterface
    virtual int32 GetCharacterLevel() const override;

protected:
    virtual void BeginPlay() override;
    virtual void InitAbilityActorInfo() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
    int32 Level = 1;
};
