// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraWidgetController.h"

void UAuraUserWidget::SetWidgetController(UAuraWidgetController* InWidgetController)
{
    WidgetController = InWidgetController;
    WidgetControllerSet();
}
