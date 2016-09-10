// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Build.h"

EBTNodeResult::Type UBTTask_Build::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {

		UOEdification* home = (UOEdification*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Edification"));

		if (home) {
			if(home->GetIsDestroyed())
				entity->RebuildEdification(home);

			else {
				entity->StopRebuildEdification();
				blackboard->ClearValue(blackboard->GetKeyID("Edification"));
			}
		}

		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}


