// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Ontology/EntityAIController.h"
#include "Node.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "BTTask_IsGoToNode.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UBTTask_IsGoToNode : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NodeType)
	//	Node::NodeType IsNodeType;
};
