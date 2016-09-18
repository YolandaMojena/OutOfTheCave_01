// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_ChangeToFlee.h"




EBTNodeResult::Type UBTTask_ChangeToFlee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {
		Node* n = new Node();
		n->SetNodeType(NodeType::flee); n->SetActor(((UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity")))->GetOwner()); n->SetHighPriority();
		if (entity->GetCurrentState() == UOEntity::AIState::plot) {
			entity->GetBrain()->AddInstantNode(n);
			if (entity->GetCurrentPlot()->GetMainEntity() == entity) {
				entity->GetCurrentPlot()->GetGraphPointer()->AddSplitSecondNode(entity->GetCurrentPlot()->GetGraphPointer()->Peek());
			}
		}
		else if (entity->GetCurrentState() == UOEntity::AIState::react) {
			entity->GetBrain()->AddInstantNode(n);
			entity->GetReacts()[0]->AddInstantNode(n);
			//entity->GetReacts()[0]->NextNode();
			//entity->GetBrain()->NextNode();
		}
		blackboard->ClearValue(blackboard->GetKeyID("Entity"));
		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}