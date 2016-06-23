// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_AskForHelpNode.h"

EBTNodeResult::Type UBTTask_AskForHelpNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* plotEntity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (plotEntity) {

		TArray<UOEntity*> helpers;

		for (UOEntity* e : plotEntity->GetCurrentPlot()->GetInvolvedInPlot())
			if (!helpers.Contains(e) && e!=plotEntity) helpers.Add(e);

		/*for (ORelation* r : plotEntity->GetRelationships()) {
			if (!r->GetOtherEntity()->IsPlayer && !helpers.Contains(r->GetOtherEntity())) helpers.Add(r->GetOtherEntity());
		}*/

		for (UOEntity* e : helpers) {
			e->SetMainPlotEntity(plotEntity);
			if (e->GetCurrentState() == UOEntity::State::idle)
				e->SetState(UOEntity::State::plot);
		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Succeeded;
}


