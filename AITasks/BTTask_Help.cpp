// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Help.h"

EBTNodeResult::Type UBTTask_Help::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {

		UOEntity* entityToHelp = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>("Entity");

		if (entityToHelp) {

			if (entityToHelp->GetCurrentState() == UOEntity::AIState::plot) {
				entityToHelp->GetCurrentPlot()->AddInvolvedInPlot(entity);
			}
			else if (entityToHelp->GetCurrentState() == UOEntity::AIState::react) {
				entity->AddInstantReactGraph(entityToHelp->GetReacts()[0]);
			}
			else if ((entityToHelp->GetCurrentState() == UOEntity::AIState::idle)) {
				Graph g;
				g.AddNode(entityToHelp->GetIdleGraph()->firstNode);
				entity->AddInstantReactGraph(&g);
			}

			entity->RethinkState();
			blackboard->SetValue<UBlackboardKeyType_Bool>("NodeCompleted", false);
		}
	}
	else blackboard->SetValue<UBlackboardKeyType_Bool>("NodeCompleted", false);

	return::EBTNodeResult::Succeeded;
}


