// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Give.h"

EBTNodeResult::Type UBTTask_Give::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOOwnable* targetOwnable = (UOOwnable*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Ownable"));
	UOEntity* receiver = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));

	if (entity && targetOwnable && entity->GetGrabbedItem() == targetOwnable && receiver) {
		entity->ReleaseGrabbedItem();
		receiver->ReceivePresent(targetOwnable, entity);

		return EBTNodeResult::Succeeded;
	}
	
	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("NodeCompleted"), false);
	return EBTNodeResult::Succeeded;

}




