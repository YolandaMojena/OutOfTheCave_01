// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_NodeCompleted.h"

EBTNodeResult::Type UBTTask_NodeCompleted::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass <UOEntity>();

	if (entity) {
		//CONTROLAR DESDE LA BLACKBOARD, NO EN EL CEREBRO
		//entity->NodeCompleted(entity->brain.GetNodeCompleted());
		//entity->brain.SetNodeCompleted(true);
	}
	return EBTNodeResult::Succeeded;
}
