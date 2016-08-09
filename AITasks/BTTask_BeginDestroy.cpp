// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_BeginDestroy.h"


EBTNodeResult::Type UBTTask_BeginDestroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {
		entity->Attack();
	}


	return EBTNodeResult::Succeeded;
}

