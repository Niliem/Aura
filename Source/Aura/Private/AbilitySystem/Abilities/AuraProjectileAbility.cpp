// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Actor/AuraProjectileActor.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileAbility::SpawnProjectile()
{
    check(ProjectileActorClass);
    if (TScriptInterface<ICombatInterface> CombatInterface = GetAvatarActorFromActorInfo())
    {
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(CombatInterface->GetCombatSocketLocation());
        AAuraProjectileActor* Projectile =
            GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        Projectile->FinishSpawning(SpawnTransform);
    }
}
