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
		for (UOEntity* e : plotEntity->GetCurrentPlot()->GetInvolvedInPlot()) {
			if (e && !helpers.Contains(e) && e != plotEntity) {
				helpers.Add(e);
			}
		}

		if (plotEntity->GetPersonality()->GetSocial() > 65) {
			// Add by relationships
			for (ORelation* r : plotEntity->GetRelationships()) {
				UOEntity* potentialHelper = r->GetOtherEntity();

				if (!potentialHelper->IsPlayer && !helpers.Contains(potentialHelper) 
					&& potentialHelper->GetRelationWith(plotEntity) 
					&& potentialHelper->GetRelationWith(plotEntity)->GetAppreciation() > ORelation::HIGH_APPRECIATION  
					&& FVector(plotEntity->GetOwner()->GetActorLocation() - potentialHelper->GetOwner()->GetActorLocation()).Size() <= searchRatio
					&& potentialHelper->GetMainPlotEntity() == nullptr
					&& potentialHelper->GetCurrentState() == UOEntity::AIState::idle) {

					plotEntity->GetCurrentPlot()->AddInvolvedInPlot(potentialHelper);
					helpers.Add(r->GetOtherEntity());
				}			
			}
		}

		// Check if the entity is free to be involved
		for(UOEntity* e : helpers) {

			if (!e->GetIsNumb() && !e->GetMainPlotEntity() ) {
				e->ChangeNotoriety(+1);
				e->SetMainPlotEntity(plotEntity);
				e->RethinkState();
			}
			else {
				plotEntity->GetCurrentPlot()->DeleteFromInvolved(e);
			}
		}

		return EBTNodeResult::Succeeded;
	}
	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}


