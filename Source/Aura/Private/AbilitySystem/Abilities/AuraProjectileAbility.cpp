// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Actor/AuraProjectileActor.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!HasAuthority(&ActivationInfo))
        return;

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
