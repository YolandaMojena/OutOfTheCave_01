// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_NodeCompleted.h"

EBTNodeResult::Type UBTTask_NodeCompleted::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass <UOEntity>();

	if (entity) {

		ClearBlackboard(blackboard);
		entity->NodeCompleted(blackboard->GetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk")));
		blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), true);
	}
	return EBTNodeResult::Succeeded;
}

void UBTTask_NodeCompleted::ClearBlackboard(UBlackboardComponent* blackboard) {

	blackboard->ClearValue(blackboard->GetKeyID("Actor"));
	blackboard->ClearValue(blackboard->GetKeyID("Item"));
	blackboard->ClearValue(blackboard->GetKeyID("Ownable"));
	blackboard->ClearValue(blackboard->GetKeyID("Edification"));
	blackboard->ClearValue(blackboard->GetKeyID("Entity"));
	blackboard->ClearValue(blackboard->GetKeyID("AffordableUse"));
	blackboard->ClearValue(blackboard->GetKeyID("Position"));
	blackboard->ClearValue(blackboard->GetKeyID("BoolKey"));
	blackboard->ClearValue(blackboard->GetKeyID("FloatKey"));
	blackboard->ClearValue(blackboard->GetKeyID("Residence"));
	
}
