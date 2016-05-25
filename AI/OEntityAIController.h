// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Entity.h"
#include "OEntityAIController.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API AOEntityAIController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(transient)
	class UBlackboardComponent* blackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* behaviorTreeComp;

public:

	AOEntityAIController();

	virtual void Possess(APawn* InPawn) override;

	uint8 targetKeyId;

};
