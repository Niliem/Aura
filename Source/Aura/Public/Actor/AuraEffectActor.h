// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
    ApplyOnOverlap,
    ApplyOnEndOverlap,
    DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
    RemoveOnEndOverlap,
    DoNotRemove
};

UENUM(BlueprintType)
enum class EActorDestructionPolicy : uint8
{
    DestroyOnOverlap,
    DestroyOnEndOverlap,
    DoNotDestroy
};

USTRUCT(BlueprintType)
struct FAppliedEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    TSubclassOf<UGameplayEffect> EffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    float EffectLevel = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    int32 StackRemovalCount = -1;
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
    GENERATED_BODY()

public:
    AAuraEffectActor();

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void OnOverlap(AActor* TargetActor);

    UFUNCTION(BlueprintCallable)
    void OnEndOverlap(AActor* TargetActor);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TArray<FAppliedEffect> EffectsToApply;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    EActorDestructionPolicy ActorDestructionPolicy = EActorDestructionPolicy::DoNotDestroy;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TArray<TSubclassOf<AActor>> AllowedClasses;

private:
    void ApplyEffectToTarget(AActor* TargetActor, const FAppliedEffect& Effect);
    void RemoveEffectFromTarget(AActor* TargetActor, const FAppliedEffect& Effect);

    TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

    TArray<AActor*> OverlapedActors;

    bool IsAllowedClass(AActor* TargetActor);
};
