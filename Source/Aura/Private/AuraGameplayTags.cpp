// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Strength, "Attributes.Primary.Strength", "Increases physical damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence", "Increases magical damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Resilience, "Attributes.Primary.Resilience", "Increases Armor and Armor Penetration");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Primary_Vigor, "Attributes.Primary.Vigor", "Increases Health");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "Reduces damage taken, improves Block Chance");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration", "Ignores Percentage of enemy Armor, increases Critical Hit Chance");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance", "Chance to cut incoming damage in half");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", "Chance to double damage plus critical hit bonus");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "Bonus damage added when a critical hit is scored");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance", "Reduces Critical Hit Chance of attacking enemies");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "Amount of Health regenerated every 1 second");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration", "Amount of Mana regenerated every 1 second");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "Maximum amount of Health obtainable");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana", "Maximum amount of Mana obtainable");

UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Vital_Health, "Attributes.Vital.Health");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attributes_Vital_Mana, "Attributes.Vital.Mana");

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_LMB, "Input.LMB", "Input for Left Mouse Button");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_RMB, "Input.RMB", "Input for Right Mouse Button");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_1, "Input.1", "Input for 1 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_2, "Input.2", "Input for 2 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_3, "Input.3", "Input for 3 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_4, "Input.4", "Input for 4 key");