// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Cultivate.h"


EBTNodeResult::Type UBTTask_Cultivate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass <UOEntity>();

	if (entity) {

		if (entity->_currentTime > blackboard->GetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("Daytime"))) {
			entity->brain.SetNodeCompleted(true);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::InProgress;
}

