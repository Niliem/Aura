// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Actor/AuraProjectileActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AuraGameplayTags.h"

void UAuraProjectileAbility::SpawnProjectile(const FVector& TargetLocation)
{
    check(ProjectileActorClass);

    const FVector SocketLocation = FVector::ZeroVector;
    if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
        ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
    FRotator Rotation = (TargetLocation - SocketLocation).Rotation();

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(SocketLocation);
    SpawnTransform.SetRotation(Rotation.Quaternion());

    AAuraProjectileActor* Projectile =
        GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
    FGameplayEffectContextHandle ContextHandle = SourceAbilitySystemComponent->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(Projectile);
    TArray<TWeakObjectPtr<AActor>> Actors;
    Actors.Add(Projectile);
    ContextHandle.AddActors(Actors);
    FHitResult HitResult;
    HitResult.Location = TargetLocation;
    ContextHandle.AddHitResult(HitResult);

    const FGameplayEffectSpecHandle GameplayEffectSpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);

    for (const auto& Damage : DamageTypes)
    {
        const float ScaledDamage = Damage.Value.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle, Damage.Key, ScaledDamage);
    }

    Projectile->DamageEffectSpecHandle = GameplayEffectSpecHandle;

    Projectile->FinishSpawning(SpawnTransform);
}
