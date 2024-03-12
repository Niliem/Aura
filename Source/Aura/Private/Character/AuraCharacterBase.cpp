// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterGameplayInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AuraGameplayTags.h"
#include "Aura/Aura.h"

AAuraCharacterBase::AAuraCharacterBase()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetCapsuleComponent()->SetGenerateOverlapEvents(false);
    GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
    GetMesh()->SetGenerateOverlapEvents(true);
    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
    Weapon->SetupAttachment(GetMesh(), WeaponHandSocketName);
    Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AAuraCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitializeDefaultAttributes()
{
    if (!HasAuthority())
        return;

    if (CharacterGameplayInfo)
        CharacterGameplayInfo->GiveEffects(GetAbilitySystemComponent(), GetCharacterLevel());
}

void AAuraCharacterBase::InitializeDefaultAbilities()
{
    if (!HasAuthority())
        return;

    if (CharacterGameplayInfo)
        CharacterGameplayInfo->GiveAbilities(GetAbilitySystemComponent(), GetCharacterLevel());
}

void AAuraCharacterBase::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> EffectClassToApply, const float Level) const
{
    if (!IsValid(GetAbilitySystemComponent()))
        return;
    if (!IsValid(EffectClassToApply))
        return;

    FGameplayEffectContextHandle GameplayEffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
    GameplayEffectContextHandle.AddSourceObject(this);
    const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClassToApply, Level, GameplayEffectContextHandle);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
}

void AAuraCharacterBase::Dissolve()
{
    if (IsValid(MeshDissolveMaterialInstance))
    {
        UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MeshDissolveMaterialInstance, this);
        GetMesh()->SetMaterial(0, DynamicMaterialInstance);
        AnimateMeshDissolveMaterial(DynamicMaterialInstance);
    }
    if (IsValid(WeaponDissolveMaterialInstance))
    {
        UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
        Weapon->SetMaterial(0, DynamicMaterialInstance);
        AnimateWeaponDissolveMaterial(DynamicMaterialInstance);
    }
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) const
{
    if (SocketTag.MatchesTagExact(AuraGameplayTags::Socket_Weapon) && IsValid(Weapon))
    {
        return Weapon->GetSocketLocation(WeaponTipSocketName);
    }
    if (SocketTag.MatchesTagExact(AuraGameplayTags::Socket_LeftHand))
    {
        return GetMesh()->GetSocketLocation(LeftHandSocketName);
    }
    if (SocketTag.MatchesTagExact(AuraGameplayTags::Socket_RightHand))
    {
        return GetMesh()->GetSocketLocation(RightHandSocketName);
    }
    return FVector();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
    return bIsDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
    return this;
}

void AAuraCharacterBase::Die_Implementation()
{
    Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
    MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
    Weapon->SetSimulatePhysics(true);
    Weapon->SetEnableGravity(true);
    Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetEnableGravity(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Dissolve();

    bIsDead = true;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
    return AttributeSet;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation() const
{
    return HitReactMontage;
}
