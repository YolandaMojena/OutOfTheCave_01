// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_IsGoToNode.h"




EBTNodeResult::Type UBTTask_IsGoToNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AEntityAIController* ownerEntityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());

	if (ownerEntityController) {
		//NodeType nodeType;
	}

	return EBTNodeResult::Failed;
}