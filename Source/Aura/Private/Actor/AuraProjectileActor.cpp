// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AuraProjectileActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"

AAuraProjectileActor::AAuraProjectileActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    SetRootComponent(Sphere);

    Sphere->SetCollisionObjectType(ECC_Projectile);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    ProjectileMovementComponent->InitialSpeed = 550.0f;
    ProjectileMovementComponent->MaxSpeed = 5500.0f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AAuraProjectileActor::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(LifeSpan);
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectileActor::OnSphereOverlap);
}

void AAuraProjectileActor::Destroyed()
{
    if (!bHit && !HasAuthority())
    {
        OnHit();
    }
    Super::Destroyed();
}

void AAuraProjectileActor::OnHit()
{
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
}

void AAuraProjectileActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!IsValid(GetOwner()))
    {
        bHit = true;
        Destroy();
        return;
    }

    if (GetOwner() == OtherActor)
    {
        return;
    }

    if (UAuraAbilitySystemLibrary::IsOnSameTeam(GetOwner(), OtherActor))
    {
        return;
    }

    OnHit();

    if (HasAuthority())
    {
        if (IsValid(DamageEffectParams.SourceAbilitySystemComponent))
        {
            if (auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
            {
                DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
                DamageEffectParams.TargetAbilitySystemComponent = TargetAbilitySystemComponent;
                UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
            }
        }

        Destroy();
    }
    else
    {
        SetActorHiddenInGame(true);
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        bHit = true;
    }
}
