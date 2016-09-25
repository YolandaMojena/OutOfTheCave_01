// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Help.h"

EBTNodeResult::Type UBTTask_Help::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = nullptr;
	entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = nullptr;
	blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = nullptr;
	entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {

		UOEntity* entityToHelp = nullptr;
		entityToHelp = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>("Entity");

		if (entityToHelp) {

			entity->ClearState();

			if (entityToHelp->GetCurrentState() == UOEntity::AIState::plot) {
				if (entityToHelp->GetCurrentPlot() && entityToHelp->GetCurrentPlot()->GetMainEntity() == entityToHelp) {
					entityToHelp->GetCurrentPlot()->AddInvolvedInPlot(entity);
					entity->SetMainPlotEntity(entityToHelp);
				}
				else if (entityToHelp->GetMainPlotEntity() && entityToHelp->GetMainPlotEntity()->GetCurrentPlot()) {
					entityToHelp->GetMainPlotEntity()->GetCurrentPlot()->AddInvolvedInPlot(entity);
					entity->SetMainPlotEntity(entityToHelp->GetMainPlotEntity());
				}
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
		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("NodeCompleted"), false);
	return EBTNodeResult::Succeeded;	
}


