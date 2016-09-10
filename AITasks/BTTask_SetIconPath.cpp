// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_SetIconPath.h"

EBTNodeResult::Type UBTTask_SetIconPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* ownerEntityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	if (ownerEntityController){
		
		UOCivilian* civilianComp = ownerEntityController->GetPawn()->FindComponentByClass<UOCivilian>();

		if(civilianComp)
			civilianComp->currentIconPath = taskIconPath;

		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}


