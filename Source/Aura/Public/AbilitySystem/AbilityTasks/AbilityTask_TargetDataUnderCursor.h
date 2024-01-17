// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "AbilityTask_TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataUnderCursorDelegate, const FVector&, Data);

/**
 *
 */
UCLASS()
class AURA_API UAbilityTask_TargetDataUnderCursor : public UAbilityTask
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FTargetDataUnderCursorDelegate ValidData;

    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderCursor", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UAbilityTask_TargetDataUnderCursor* CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility);

private:
    virtual void Activate() override;
};
