// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_StopFight.h"


EBTNodeResult::Type UBTTask_StopFight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	
	UOEntity* other = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
	AEntityAIController* otherEntityController = other->GetEntityAIController();
	otherEntityController->entityBlackboard->ClearValue(blackboard->GetKeyID("Entity"));

	return EBTNodeResult::Succeeded;
}

