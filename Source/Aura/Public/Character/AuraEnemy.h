// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTags.h"
#include "AuraEnemy.generated.h"

class UAuraUserWidget;
class UWidgetComponent;
class UEnemyWidgetController;
class UBehaviorTree;
class AAuraAIController;

/**
 *
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
    GENERATED_BODY()

public:
    AAuraEnemy();

    virtual void PossessedBy(AController* NewController) override;

    // Inherited via IEnemyInterface
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;
    virtual void SetCombatTarget_Implementation(AActor* TargetActor) override;
    virtual AActor* GetCombatTarget_Implementation() const override;

    // Inherited via ICombatInterface
    virtual int32 GetCharacterLevel() const override;
    virtual void Die() override;

    void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
    int32 Level = 1;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bHitReacting = false;

    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    TObjectPtr<AActor> CombatTarget = nullptr;

private:
    virtual void InitAbilityActorInfo() override;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UAuraUserWidget> HealthWidgetClass;

    UPROPERTY()
    TObjectPtr<UWidgetComponent> HealthWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UEnemyWidgetController> EnemyWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<UEnemyWidgetController> EnemyWidgetController;

    UPROPERTY(EditAnywhere, Category = "AI")
    TObjectPtr<UBehaviorTree> BehaviorTree;

    UPROPERTY()
    TObjectPtr<AAuraAIController> AuraAIController;
};
