// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_AskTrollForHelp.h"

EBTNodeResult::Type UBTTask_AskTrollForHelp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* plotEntity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (plotEntity) {

		UOEntity* troll = plotEntity->GetOwner()->GetWorld()->GetFirstPlayerController()->FindComponentByClass<UOEntity>();

		if (troll && FVector(plotEntity->GetOwner()->GetActorLocation() - troll->GetOwner()->GetActorLocation()).Size() < searchRatio) {

			ORelation* relation = troll->GetRelationWith(plotEntity);
			if (relation && relation->GetAppreciation() >= relation->HIGH_APPRECIATION)
				entityController->MoveToActor(troll->GetOwner());
		}

		//plotEntity->brain.SetNodeCompleted(true);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}


