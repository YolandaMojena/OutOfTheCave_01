// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_ReleaseGrabbedItem.h"




EBTNodeResult::Type UBTTask_ReleaseGrabbedItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity && entity->HasGrabbedItem()) {
		entity->ReleaseGrabbedItem();
		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}