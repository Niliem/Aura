// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Actor/AuraProjectileActor.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UAuraProjectileAbility::FormatAbilityDescription(int32 Level, FText& OutDescription)
{
    Super::FormatAbilityDescription(Level, OutDescription);

    OutDescription = FText::FormatNamed(OutDescription,
    "Proj_Num",
    GetNumProjectiles(Level),
    "Proj_Num_Next",
    GetNumProjectiles(Level + 1)
    );
}

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

    const TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, GetNumProjectiles(GetAbilityLevel()));

    for (const FRotator& SpacedRotation : Rotations)
    {
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SocketLocation);
        SpawnTransform.SetRotation(SpacedRotation.Quaternion());

        AAuraProjectileActor* Projectile =
            GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetAvatarActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

        if (bLaunchHomingProjectiles)
        {
            if (IsValid(HomingTarget) && HomingTarget->Implements<UCombatInterface>())
            {
                Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
            }
            else
            {
                Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
                Projectile->HomingTargetSceneComponent->SetWorldLocation(TargetLocation);
                Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
            }
            Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::RandRange(HomingAcceleration.GetLowerBoundValue(), HomingAcceleration.GetUpperBoundValue());
            Projectile->ProjectileMovementComponent->bIsHomingProjectile = true;
        }

        Projectile->FinishSpawning(SpawnTransform);
    }
}

int32 UAuraProjectileAbility::GetNumProjectiles_Implementation(int32 Level) const
{
    return NumProjectiles;
}
