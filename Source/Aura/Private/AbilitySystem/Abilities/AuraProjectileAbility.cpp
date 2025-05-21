// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Actor/AuraProjectileActor.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraProjectileAbility::SpawnProjectiles(const FVector& TargetLocation, FGameplayTag Socket, AActor* HomingTarget, bool bOverridePitch, float PitchOverride)
{
    if (!GetAvatarActorFromActorInfo()->HasAuthority())
        return;

    check(ProjectileActorClass);

    FVector SocketLocation = FVector::ZeroVector;
    if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
        SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), Socket);
    FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
    if (bOverridePitch)
    {
        Rotation.Pitch = PitchOverride;
    }

    const FVector Forward = Rotation.Vector();

    const TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

    for (const FRotator& SpacedRotation : Rotations)
    {
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SocketLocation);
        SpawnTransform.SetRotation(SpacedRotation.Quaternion());

        AAuraProjectileActor* Projectile =
            GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetAvatarActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

        Projectile->FinishSpawning(SpawnTransform);
    }
}
