// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_NodeCompleted.h"

EBTNodeResult::Type UBTTask_NodeCompleted::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass <UOEntity>();

	if (entity) {
		entity->NodeCompleted(blackboard->GetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk")));
		// Reset value to default
		blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), true);
	}
	return EBTNodeResult::Succeeded;
}
