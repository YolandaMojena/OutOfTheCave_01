// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Defend.h"

EBTNodeResult::Type UBTTask_Defend::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOEntity* other = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));

	if (entity && other) {

		if ((entity->GetOwner()->GetActorLocation() - other->GetOwner()->GetActorLocation()).Size() < ATTACK_RANGE)
		{
			blackboard->ClearValue(blackboard->GetKeyID("Actor"));

			Node* n = new Node();
			n->SetNodeType(NodeType::attack); n->SetEntity(other);
			if (entity->GetCurrentState() == UOEntity::AIState::plot) {
				entity->GetBrain()->AddInstantNode(n);
				if (entity->GetCurrentPlot()->GetMainEntity() == entity) {
					entity->GetCurrentPlot()->GetGraphPointer()->AddSplitSecondNode(entity->GetCurrentPlot()->GetGraphPointer()->Peek());
				}
			}
			else if (entity->GetCurrentState() == UOEntity::AIState::react) {
				entity->GetBrain()->AddInstantNode(n);
				entity->GetReacts()[0]->AddInstantNode(n);
			}

			return EBTNodeResult::Succeeded;

		}
	}

	return EBTNodeResult::Failed;
}


