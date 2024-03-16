// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace AuraGameplayTags
{
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Strength);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Intelligence);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Resilience);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Vigor);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_Armor);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_ArmorPenetration);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_BlockChance);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_CriticalHitChance);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_CriticalHitDamage);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_CriticalHitResistance);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_HealthRegeneration);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_ManaRegeneration);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_MaxHealth);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_MaxMana);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Physical);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Fire);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Arcane);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Lightning);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Vital_Health);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Vital_Mana);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Melee);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Ranged_Fire_FireBolt);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReact);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Passive_Negative_Physical_Bleed);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_HitReact);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockedHit);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_CriticalHit);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Status_Negative_Physical_Bleed);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Status_Negative_Elemental_Burn);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Status_Healthy);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Status_Dead);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Status_Immortal);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Status_Invulnerable);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Physical);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Elemental_Fire);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Elemental_Arcane);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Elemental_Lightning);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Melee);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Ranged_Slinghot);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Ranged_FireBolt);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Socket_Weapon);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Socket_LeftHand);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Socket_RightHand);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Socket_Tail);
};  // namespace AuraGameplayTags