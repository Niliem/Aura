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
    if (!IsAllowedClass(TargetActor))
        return;

    if (OverlapedActors.Contains(TargetActor))
        return;

    OverlapedActors.AddUnique(TargetActor);
    for (const auto& EffectToApply : EffectsToApply)
    {
        if (EffectToApply.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
        {
            ApplyEffectToTarget(TargetActor, EffectToApply);
        }
    }

    if (ActorDestructionPolicy == EActorDestructionPolicy::DestroyOnOverlap)
        Destroy();
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
    if (!IsAllowedClass(TargetActor))
        return;

    OverlapedActors.Remove(TargetActor);
    for (const auto& EffectToApply : EffectsToApply)
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

    if (ActorDestructionPolicy == EActorDestructionPolicy::DestroyOnEndOverlap)
        Destroy();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const FAppliedEffect& Effect)
{
    UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (TargetAbilitySystemComponent == nullptr)
        return;

    check(Effect.EffectClass);

    FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
    EffectContextHandle.AddSourceObject(this);

    const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(Effect.EffectClass, Effect.EffectLevel, EffectContextHandle);
    const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

    const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
    if (bIsInfinite && Effect.EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
    {
        ActiveEffectHandles.Add(ActiveEffectHandle, TargetAbilitySystemComponent);
    }
}

void AAuraEffectActor::RemoveEffectFromTarget(AActor* TargetActor, const FAppliedEffect& Effect)
{
    UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!IsValid(TargetAbilitySystemComponent))
        return;

    TArray<FActiveGameplayEffectHandle> HandlesToRemove;
    for (const auto& HandlePair : ActiveEffectHandles)
    {
        if (TargetAbilitySystemComponent == HandlePair.Value)
        {
            auto ActiveEffectFromHandle = UAbilitySystemBlueprintLibrary::GetGameplayEffectFromActiveEffectHandle(HandlePair.Key);
            if (ActiveEffectFromHandle && ActiveEffectFromHandle->IsA(Effect.EffectClass))
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

bool AAuraEffectActor::IsAllowedClass(AActor* TargetActor)
{
    for (auto AllowedClass : AllowedClasses)
    {
        if (TargetActor->IsA(AllowedClass))
        {
            return true;
        }
    }
    return false;
}
