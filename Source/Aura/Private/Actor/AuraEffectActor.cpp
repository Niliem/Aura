// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

AAuraEffectActor::AAuraEffectActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
    Super::BeginPlay();
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
    for (auto& EffectToApply : EffectsToApply)
    {
        if (EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
        {
            ApplyEffectToTarget(TargetActor, EffectToApply);
        }
    }
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
    for (auto& EffectToApply : EffectsToApply)
    {
        if (EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
        {
            ApplyEffectToTarget(TargetActor, EffectToApply);
        }
        if (EffectToApply.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
        {
            RemoveEffectFromTarget(TargetActor, EffectToApply);
        }
    }
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const FAppliedEffect& Effect)
{
    UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (TargetAbilitySystemComponent == nullptr)
        return;

    check(Effect.GameplayEffectClass);

    FGameplayEffectContextHandle GameplayEffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
    GameplayEffectContextHandle.AddSourceObject(this);

    const FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(Effect.GameplayEffectClass, 1.0f, GameplayEffectContextHandle);

    const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

    if (Effect.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
    {
        ActiveEffectHandles.Add(ActiveGameplayEffectHandle, TargetAbilitySystemComponent);
    }
}

void AAuraEffectActor::RemoveEffectFromTarget(AActor* TargetActor, const FAppliedEffect& Effect)
{
    UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!IsValid(TargetAbilitySystemComponent))
        return;

    TArray<FActiveGameplayEffectHandle> HandlesToRemove;
    for (auto& HandlePair : ActiveEffectHandles)
    {
        if (UAbilitySystemBlueprintLibrary::GetGameplayEffectFromActiveEffectHandle(HandlePair.Key)->IsA(Effect.GameplayEffectClass))
        {
            if (TargetAbilitySystemComponent == HandlePair.Value)
            {
                TargetAbilitySystemComponent->RemoveActiveGameplayEffect(HandlePair.Key, Effect.StackRemovalCount);
                HandlesToRemove.Add(HandlePair.Key);
            }
        }
    }
    for (auto& Handle : HandlesToRemove)
    {
        ActiveEffectHandles.FindAndRemoveChecked(Handle);
    }
}
