// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraGameplayTags.h"

namespace AuraGameplayTags
{
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Strength, "Attribute.Primary.Strength", "Increases physical damage");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Intelligence, "Attribute.Primary.Intelligence", "Increases magical damage");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Resilience, "Attribute.Primary.Resilience", "Increases Armor and Armor Penetration");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Vigor, "Attribute.Primary.Vigor", "Increases Health");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_Armor, "Attribute.Secondary.Armor", "Reduces damage taken, improves Block Chance");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_ArmorPenetration, "Attribute.Secondary.ArmorPenetration", "Ignores Percentage of enemy Armor, increases Critical Hit Chance");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_BlockChance, "Attribute.Secondary.BlockChance", "Chance to cut incoming damage in half");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_CriticalHitChance, "Attribute.Secondary.CriticalHitChance", "Chance to double damage plus critical hit bonus");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_CriticalHitDamage, "Attribute.Secondary.CriticalHitDamage", "Bonus damage added when a critical hit is scored");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_CriticalHitResistance, "Attribute.Secondary.CriticalHitResistance", "Reduces Critical Hit Chance of attacking enemies");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_HealthRegeneration, "Attribute.Secondary.HealthRegeneration", "Amount of Health regenerated every 1 second");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_ManaRegeneration, "Attribute.Secondary.ManaRegeneration", "Amount of Mana regenerated every 1 second");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_MaxHealth, "Attribute.Secondary.MaxHealth", "Maximum amount of Health obtainable");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_MaxMana, "Attribute.Secondary.MaxMana", "Maximum amount of Mana obtainable");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Resistance_Physical, "Attribute.Resistance.Physical", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Resistance_Fire, "Attribute.Resistance.Fire", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Resistance_Arcane, "Attribute.Resistance.Arcane", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Resistance_Lightning, "Attribute.Resistance.Lightning", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Vital_Health, "Attribute.Vital.Health", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Vital_Mana, "Attribute.Vital.Mana", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Meta_IncomingXP, "Attribute.Meta.IncomingXP", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability, "Ability", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Status, "Ability.Status", "Base for ability statuses");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Status_Locked, "Ability.Status.Locked", "Locked status for ability");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Status_Eligible, "Ability.Status.Eligible", "Eligible status for ability");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Status_Unlocked, "Ability.Status.Unlocked", "Unlocked status for ability");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Status_Equipped, "Ability.Status.Equipped", "Equipped status for ability");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type, "Ability.Type", "Base for ability types");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_None, "Ability.Type.None", "None type for ability");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Active, "Ability.Type.Active", "Active type for ability");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Passive, "Ability.Type.Passive", "Passive type for ability");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Physical_Melee, "Ability.Physical.Melee", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Physical_Shoot, "Ability.Physical.Shoot", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Fire_FireBolt, "Ability.Fire.FireBolt", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Fire_FireBlast, "Ability.Fire.FireBlast", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Lightning_Electrocute, "Ability.Lightning.Electrocute", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Arcane_ArcaneShards, "Ability.Arcane.ArcaneShards", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Arcane_Summon, "Ability.Arcane.Summon", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_HitReact, "Ability.HitReact", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Passive_Negative_Physical_Bleed, "Ability.Passive.Negative.Physical.Bleed", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_HitReact, "Effect.HitReact", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_BlockedHit, "Effect.BlockedHit", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_CriticalHit, "Effect.CriticalHit", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_Status_Negative_Physical_Bleed, "Effect.Status.Negative.Physical.Bleed", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_Status_Negative_Elemental_Burn, "Effect.Status.Negative.Elemental.Burn", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Status_Healthy, "Character.Status.Healthy", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Status_Dead, "Character.Status.Dead", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Status_Immortal, "Character.Status.Immortal", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_Status_Invulnerable, "Character.Status.Invulnerable", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageType, "DamageType", "Parent tag for all damage types");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageType_Physical, "DamageType.Physical", "Physical damage type");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageType_Elemental_Fire, "DamageType.Elemental.Fire", "Fire damage type");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageType_Elemental_Arcane, "DamageType.Elemental.Arcane", "Arcane damage type");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageType_Elemental_Lightning, "DamageType.Elemental.Lightning", "Lightning damage type");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay effects.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_IncomingXP, "SetByCaller.IncomingXP", "SetByCaller tag used by IncomingXP gameplay effects.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_AttributePoints, "SetByCaller.AttributePoints", "SetByCaller tag used by AttributePoints gameplay effects.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_SpellPoints, "SetByCaller.SpellPoints", "SetByCaller tag used by SpellPoints gameplay effects.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Strength, "SetByCaller.Strength", "SetByCaller tag used by Strength gameplay effects.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Intelligence, "SetByCaller.Intelligence", "SetByCaller tag used by Intelligence gameplay effects.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Resilience, "SetByCaller.Resilience", "SetByCaller tag used by Resilience gameplay effects.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Vigor, "SetByCaller.Vigor", "SetByCaller tag used by Vigor gameplay effects.");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag, "InputTag", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LMB, "InputTag.LMB", "Input Tag for Left Mouse Button");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_RMB, "InputTag.RMB", "Input Tag for Right Mouse Button");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_1, "InputTag.1", "Input Tag for 1 key");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_2, "InputTag.2", "Input Tag for 2 key");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_3, "InputTag.3", "Input Tag for 3 key");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_4, "InputTag.4", "Input Tag for 4 key");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Passive_1, "InputTag.Passive.1", "Input Tag for Passive 1");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Passive_2, "InputTag.Passive.2", "Input Tag for Passive 2");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Montage_Melee, "GameplayEvent.Montage.Melee", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Montage_Ranged_Slingshot, "GameplayEvent.Montage.Ranged.Slingshot", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Montage_Ranged_FireBolt, "GameplayEvent.Montage.Ranged.FireBolt", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_LevelUp, "GameplayEvent.LevelUp", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_IncomingXP, "GameplayEvent.IncomingXP", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_AttributePoints, "GameplayEvent.AttributePoints", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_SpellPoints, "GameplayEvent.SpellPoints", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Strength, "GameplayEvent.Strength", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Intelligence, "GameplayEvent.Intelligence", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Resilience, "GameplayEvent.Resilience", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Vigor, "GameplayEvent.Vigor", "");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Socket_Weapon, "Socket.Weapon", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Socket_LeftHand, "Socket.LeftHand", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Socket_RightHand, "Socket.RightHand", "");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Socket_Tail, "Socket.Tail", "");
}  // namespace AuraGameplayTags
