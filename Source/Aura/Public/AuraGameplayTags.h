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

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Meta_IncomingXP);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Locked);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Eligible);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Unlocked);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Equipped);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_None);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Active);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Passive);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Physical_Melee);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Physical_Shoot);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Fire_FireBolt);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Fire_FireBlast);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Lightning_Electrocute);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Arcane_ArcaneShards);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Arcane_Summon);

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
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_IncomingXP);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_AttributePoints);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Strength);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Intelligence);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Resilience);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Vigor);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_1);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_2);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Melee);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Ranged_Slingshot);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Ranged_FireBolt);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_LevelUp);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_IncomingXP);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_AttributePoints);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Strength);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Intelligence);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Resilience);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Vigor);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Socket_Weapon);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Socket_LeftHand);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Socket_RightHand);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Socket_Tail);
};  // namespace AuraGameplayTags
