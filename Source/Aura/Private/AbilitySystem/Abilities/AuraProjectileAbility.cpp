// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Actor/AuraProjectileActor.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileAbility::SpawnProjectile(const FVector& TargetLocation)
{
    check(ProjectileActorClass);
    if (TScriptInterface<ICombatInterface> CombatInterface = GetAvatarActorFromActorInfo())
    {
        const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
        FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
        Rotation.Pitch = 0.0f;

        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SocketLocation);        
        SpawnTransform.SetRotation(Rotation.Quaternion());

        AAuraProjectileActor* Projectile =
            GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        Projectile->FinishSpawning(SpawnTransform);
    }
}
