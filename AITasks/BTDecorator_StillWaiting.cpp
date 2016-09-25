// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTDecorator_StillWaiting.h"

bool UBTDecorator_StillWaiting::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = nullptr;
	entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOEntity* otherEntity = nullptr;
	otherEntity = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
	AActor* targetActor = (AActor*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Actor"));

	if (entity->IsValidItem() && otherEntity->IsValidItem() && targetActor) {

		if (otherEntity->GetCurrentPlot()->GetGraphPointer()->Peek()->nBlackboard.actor != targetActor) {
			blackboard->ClearValue(blackboard->GetKeyID("Actor"));
			return false;
		}
		return true;
	}

	return false;
}





