// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

constexpr auto ECC_Projectile = ECollisionChannel::ECC_GameTraceChannel1;

// Stencil values
     
constexpr auto  STENCIL_RED = 250;
constexpr auto  STENCIL_BLUE = 251;
constexpr auto  STENCIL_TAN = 252;
     
UENUM(BlueprintType)
enum class EStencilValue : uint8
{
    Default,
    Red = STENCIL_RED,
    Blue = STENCIL_BLUE,
    Tan = STENCIL_TAN
};