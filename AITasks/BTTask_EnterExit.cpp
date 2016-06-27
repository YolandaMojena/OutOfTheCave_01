// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_EnterExit.h"


EBTNodeResult::Type UBTTask_EnterExit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOResidence* residence = (UOResidence*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Residence"));
	

	if (entity && residence) {
		


		return::EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

