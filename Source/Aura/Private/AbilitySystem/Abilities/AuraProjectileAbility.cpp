// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Actor/AuraProjectileActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AuraGameplayTags.h"

void UAuraProjectileAbility::SpawnProjectile(const FVector& TargetLocation, FGameplayTag Socket)
{
    if (!GetAvatarActorFromActorInfo()->HasAuthority())
        return;

    check(ProjectileActorClass);

    FVector SocketLocation = FVector::ZeroVector;
    if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
        SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), Socket);
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

    Projectile->DamageEffectSpecHandle = MakeDamageEffectSpecHandle(ContextHandle);

    Projectile->FinishSpawning(SpawnTransform);
}
