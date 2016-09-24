// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_StopFight.h"


EBTNodeResult::Type UBTTask_StopFight::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	
	UOEntity* other = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));

	if (!other->IsValidItem()) {
		blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
		return EBTNodeResult::Succeeded;
	}

	if (other->IsPlayer) {
		blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("BoolKey"), true);
		return EBTNodeResult::Succeeded;
	}

	AEntityAIController* otherEntityController = other->GetEntityAIController();

	UItem* protege;// = (UItem*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Item"));
	//if(!protege)
		protege = (UItem*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Residence"));
	if (!protege)
		protege = (UItem*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Edification"));
	if (!protege)
		protege = (UItem*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("AnotherEntity"));

	if (!protege->IsValidItem()) {
		blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("BoolKey"), false);
	if (other->IsValidItem() && other->GetBrain() && other->GetBrain()->Peek() && otherEntityController && otherEntityController->entityBlackboard) {
		if (other->GetBrain()->Peek()->GetNodeType() == NodeType::attack
			&& other->GetBrain()->Peek()->nBlackboard.entity == (UOEntity*)protege)
		{
			otherEntityController->entityBlackboard->ClearValue(blackboard->GetKeyID("Entity"));
		}
		else if (other->GetBrain()->Peek()->GetNodeType() == NodeType::destroy
			&& other->GetBrain()->Peek()->nBlackboard.edification == (UOEdification*)protege)
		{
			otherEntityController->entityBlackboard->ClearValue(blackboard->GetKeyID("Edification"));
		}
	}
	else {
		blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	}

	return EBTNodeResult::Succeeded;
}

