// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Thing.h"
#include "DestructibleThing.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API ADestructibleThing : public AThing
{
	GENERATED_BODY()

		UPROPERTY()
		UDestructibleMesh* mesh;

};
