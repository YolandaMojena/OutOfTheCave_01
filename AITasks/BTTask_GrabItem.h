// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Ontology/EntityAIController.h"
#include "Ontology/Item.h"
#include "Ontology/OEntity.h"
#include "BTTask_GrabItem.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UBTTask_GrabItem : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_GrabItem();
	~UBTTask_GrabItem();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
