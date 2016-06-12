// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_GoToNode.h"

EBTNodeResult::Type UBTTask_GoToNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass <UOEntity>();

	if (entity) {

		FVector targetLocation = blackboard->GetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID("Position"));
		entityController->MoveToLocation(targetLocation, 50.0f, true, true, true, 0);

		entity->brain.SetNodeCompleted(true);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}




