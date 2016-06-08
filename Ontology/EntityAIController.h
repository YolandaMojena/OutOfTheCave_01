// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Node.h"
#include "EntityAIController.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API AEntityAIController : public AAIController
{
	GENERATED_BODY()

		Node* _currentNode;
	
	
};
