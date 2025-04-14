// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"


class UAbilityInfo;

/**
 *
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Abilities")
    TObjectPtr<UAbilityInfo> AbilityInfo;
};
