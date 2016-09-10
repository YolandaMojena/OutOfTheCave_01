// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_StopFight.h"


EBTNodeResult::Type UBTTask_StopFight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	
	UOEntity* other = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
	AEntityAIController* otherEntityController = other->GetEntityAIController();

	UItem* protege = (UItem*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Item"));

	if(other->GetBrain()->Peek()->GetNodeType() == NodeType::attack
		&& other->GetBrain()->Peek()->nBlackboard.entity == (UOEntity*)protege)
		otherEntityController->entityBlackboard->ClearValue(blackboard->GetKeyID("Entity"));
	else if (other->GetBrain()->Peek()->GetNodeType() == NodeType::destroy
		&& other->GetBrain()->Peek()->nBlackboard.edification == (UOEdification*)protege)
		otherEntityController->entityBlackboard->ClearValue(blackboard->GetKeyID("Edification"));


	return EBTNodeResult::Succeeded;
}

