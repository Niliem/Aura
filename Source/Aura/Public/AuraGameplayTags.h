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

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Vital_Health);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Vital_Mana);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_FireBolt);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReact);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_HitReact);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_BlockedHit);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_CriticalHit);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_FireBolt);
};  // namespace AuraGameplayTags