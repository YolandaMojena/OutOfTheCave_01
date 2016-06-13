// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_AskForHelpNode.h"

EBTNodeResult::Type UBTTask_AskForHelpNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* plotEntity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (plotEntity) {

		FVector start = plotEntity->GetOwner()->GetActorLocation();
		FVector end = start;
		TArray<FHitResult> outHits;

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, plotEntity->GetOwner());
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		plotEntity->GetOwner()->GetWorld()->SweepMultiByChannel(
			outHits,
			start,
			end,
			FQuat(),
			ECollisionChannel::ECC_Visibility,
			FCollisionShape::MakeSphere(searchRatio),
			RV_TraceParams
			);

		for (FHitResult hr : outHits) {
			UOEntity* entity = hr.GetActor()->FindComponentByClass<UOEntity>();
			if (entity && entity->IsInSight(plotEntity->GetOwner()) && entity->GetCurrentState() == UOEntity::State::idle) {
				ORelation* relation = entity->GetRelationWith(plotEntity);
				if (relation && relation->GetAppreciation() >= relation->HIGH_APPRECIATION) {
					plotEntity->GetCurrentPlot()->AddInvolvedInPlot(entity);
				}
			}
		}

		//Graph plotGraph = plotEntity->GetCurrentPlot()->GetGraph();
		/*Node* comeToEntity = new Node();
		comeToEntity->SetNodeType(NodeType::goTo);
		comeToEntity->SetPosition(plotEntity->GetOwner()->GetActorLocation());
		plotEntity->AddInstantNode(comeToEntity);*/
		//plotGraph.AddInstantNode(comeToEntity);
		//plotGraph.NextNode();
		

		for (UOEntity* e : plotEntity->GetCurrentPlot()->GetInvolvedInPlot()) {
			e->SetMainPlotEntity(plotEntity);
			e->SetState(UOEntity::State::plot);
		}

		//plotEntity->brain.SetNodeCompleted(true);
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Succeeded;
}


