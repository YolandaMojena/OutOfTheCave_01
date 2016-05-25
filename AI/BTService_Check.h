// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Entity.h"
#include "Troll.h"
#include "OEntityAIController.h"
#include "BTService_Check.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UBTService_Check : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_Check();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
