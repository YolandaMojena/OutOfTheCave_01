// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Build.h"

EBTNodeResult::Type UBTTask_Build::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {

		UOEdification* home = (UOEdification*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Edification"));
		entity->RebuildEdification(home);

		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("Build"));

		if (!home->GetIsDestroyed()) {
			entity->StopRebuildEdification();
			blackboard->ClearValue(blackboard->GetKeyID("Edification"));
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

