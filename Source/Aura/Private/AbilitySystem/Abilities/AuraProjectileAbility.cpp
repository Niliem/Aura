// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraProjectileAbility.h"
#include "Actor/AuraProjectileActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AuraGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileAbility::SpawnProjectile(const FVector& TargetLocation, FGameplayTag Socket, bool bOverridePitch, float PitchOverride)
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

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(SocketLocation);
    SpawnTransform.SetRotation(Rotation.Quaternion());

    AAuraProjectileActor* Projectile =
        GetWorld()->SpawnActorDeferred<AAuraProjectileActor>(ProjectileActorClass, SpawnTransform, GetAvatarActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

    Projectile->FinishSpawning(SpawnTransform);
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
    const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.0f, FVector::UpVector);
    const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.0f, FVector::UpVector);

    NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
    if (NumProjectiles > 1)
    {
        const float DeltaSpread = UKismetMathLibrary::SafeDivide( ProjectileSpread, (NumProjectiles - 1));

        for (int i = 0; i < NumProjectiles; ++i)
        {
            const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
            UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Direction * 75.0f, 2.0f, FLinearColor::Red, 100.0f);

        }
    }
    else
    {
        UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 75.0f, 2.0f, FLinearColor::Red, 100.0f);
    }
    UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.0f, 2.0f, FLinearColor::Green, 100.0f);
    UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.0f, 1.0f, FLinearColor::Gray, 100.0f);
    UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.0f, 1.0f, FLinearColor::Gray, 100.0f);
}
