// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor()
{
    bIsHighlighted = true;
}

void AAuraEnemy::UnHighlightActor()
{
    bIsHighlighted = false;
}
