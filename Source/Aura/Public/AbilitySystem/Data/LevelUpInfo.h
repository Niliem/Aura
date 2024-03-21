// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 LevelUpRequirement = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 AttributePointReward = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 SpellPointReward = 1;
};

/**
 *
 */
UCLASS(BlueprintType, Const)
class AURA_API ULevelUpInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    int32 GetLevelForXP(int32 XP) const;
    int32 GetXPForLevel(int32 Level) const;
    int32 GetAttributePointRewardForLevel(int32 Level) const;
    int32 GetSpellPointRewardForLevel(int32 Level) const;

protected:
    UPROPERTY(EditDefaultsOnly)
    TArray<FAuraLevelUpInfo> LevelUpInformation;
};
