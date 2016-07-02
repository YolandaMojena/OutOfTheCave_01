// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Help.h"

EBTNodeResult::Type UBTTask_Help::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {

		if (!entity->IsPlayer) {

			UOEntity* targetEntity = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));

			if (!targetEntity->GetIsNumb()) {
				Graph* originalIdle = entity->GetIdleGraph();
				entity->SetIdleGraph(targetEntity->GetBrain());
				entity->SetState(UOEntity::State::idle);
				entity->SetIdleGraph(originalIdle);

				return::EBTNodeResult::Succeeded;
			}
			else blackboard->SetValue<UBlackboardKeyType_Bool>("NodeCompleted", false);
		}
	}
	else blackboard->SetValue<UBlackboardKeyType_Bool>("NodeCompleted", false);

	return::EBTNodeResult::Succeeded;
}


