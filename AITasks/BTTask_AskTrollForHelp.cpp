// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_AskTrollForHelp.h"

EBTNodeResult::Type UBTTask_AskTrollForHelp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {

		UOEntity* troll = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UOEntity>();

		if (troll && FVector(entity->GetOwner()->GetActorLocation() - troll->GetOwner()->GetActorLocation()).Size() < searchRatio) {

			//ORelation* relation = troll->GetRelationWith(plotEntity);
			//if (relation /*&& relation->GetAppreciation() >= relation->HIGH_APPRECIATION*/)
			entityController->MoveToActor(troll->GetOwner());
		}

		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}


