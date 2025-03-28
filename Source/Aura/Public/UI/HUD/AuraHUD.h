// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;

struct FWidgetControllerParams;

/**
 *
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
    GENERATED_BODY()

public:
    UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
    UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
    USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

    void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

    UPROPERTY()
    TObjectPtr<UAuraUserWidget> OverlayWidget;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

    UPROPERTY(EditAnywhere)
    TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
};
