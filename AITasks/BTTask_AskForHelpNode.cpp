// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_AskForHelpNode.h"

EBTNodeResult::Type UBTTask_AskForHelpNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* plotEntity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (plotEntity) {

		TArray<UOEntity*> helpers;

		// Added by reports
		for (UOEntity* e : plotEntity->GetCurrentPlot()->GetInvolvedInPlot())
			if (!helpers.Contains(e) && e!=plotEntity) helpers.Add(e);

		if (plotEntity->GetPersonality()->GetSocial() > 50) {
			// Add by relationships
			for (ORelation* r : plotEntity->GetRelationships()) {
				UOEntity* potentialHelper = r->GetOtherEntity();
				if (!potentialHelper->IsPlayer && !helpers.Contains(potentialHelper) && potentialHelper->GetRelationWith(plotEntity) &&
					potentialHelper->GetRelationWith(plotEntity)->GetAppreciation()>50)
					helpers.Add(r->GetOtherEntity());
			}
		}

		// Check if the entity is free to be involved
		for(UOEntity* e : helpers) {

			if (e->GetMainPlotEntity() == nullptr) {
				e->SetMainPlotEntity(plotEntity);
				e->ChangeNotoriety(+1);
				e->RethinkState();
			}
			else {
				plotEntity->GetCurrentPlot()->DeleteFromInvolved(e);
			}
		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Succeeded;
}


