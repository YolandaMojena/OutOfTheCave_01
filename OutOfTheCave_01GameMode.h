// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "OutOfTheCave_01GameMode.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API AOutOfTheCave_01GameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	AOutOfTheCave_01GameMode(const FObjectInitializer& ObjectInitializer);
};
