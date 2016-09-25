// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTService_AttackMoveTo.h"


void UBTService_AttackMoveTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	AEntityAIController* entityController = nullptr;
	entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = nullptr;
	blackboard = entityController->entityBlackboard;

	UOEntity* entity = nullptr;
	entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	FVector entityPosition = entity->GetOwner()->GetActorLocation();
	
	UOEntity* other = nullptr;
	other = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
	AActor* otherActor = nullptr;
	otherActor = (AActor*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Actor"));
	
	//FVector targetPosition = otherActor->GetActorLocation();

	//blackboard->SetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID("Position"), targetPosition);

	if (entity->IsValidItem() && other->IsValidItem()) {

		// PARDON ENTITY?
		if (!other->IsPlayer && other->GetBrain() && other->GetBrain()->Peek() &&other->GetBrain()->Peek()->GetNodeType() == NodeType::flee) {
			ORelation* relationWithOther = entity->GetRelationWith(other);
			if (!relationWithOther)
				relationWithOther = entity->AddRelationship(other);
			if (relationWithOther && other->GetIntegrity() < relationWithOther->GetAppreciation() / 2 + entity->GetKindness() / 2 + entity->GetIntegrity() - entity->GetAggressiveness()) {
				AbortCombat(blackboard); // PARDON
			}
				
		}
		// PARDON PLAYER?
		else if (other->IsPlayer && FVector::Dist(entity->GetOwner()->GetActorLocation(), other->GetOwner()->GetActorLocation()) > 2000) {
			AbortCombat(blackboard); // PARDON
		}
		// FLEE?
		else {
			ORelation* relationWithOther = entity->GetRelationWith(other);
			if (!relationWithOther)
				relationWithOther = entity->AddRelationship(other);
			if (relationWithOther) {
				if (entity->GetBraveness() + entity->GetPride() + entity->GetIntegrity() <
					relationWithOther->GetFear() + other->GetIntegrity())
				{ 
					ChangeToFlee(entity, otherActor);
					AbortCombat(blackboard);
				}
				
			}
		}
		if (otherActor->GetActorLocation().Z < -1000) {
			AbortCombat(blackboard);
		}
	}
}


void UBTService_AttackMoveTo::AbortCombat(UBlackboardComponent* blackboard) {
	blackboard->ClearValue(blackboard->GetKeyID("Entity"));
	//blackboard->SetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID("Position"), entityPositionHack);
	blackboard->SetValue<UBlackboardKeyType_Enum>(blackboard->GetKeyID("NodeType"), static_cast<UBlackboardKeyType_Enum::FDataType>(NodeType::numb));
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow, TEXT("Trying to abort AttacMoveTo node"));
}

void UBTService_AttackMoveTo::ChangeToFlee(UOEntity* entity, AActor* horrorActor) {
	Node* n = new Node();
	n->SetNodeType(NodeType::flee); n->SetActor(horrorActor); n->SetHighPriority();
	if (entity->GetCurrentState() == UOEntity::AIState::plot) {
		entity->AddInstantNode(n);
		if (entity->GetCurrentPlot() && entity->GetCurrentPlot()->GetMainEntity() == entity) {
			entity->GetCurrentPlot()->GetGraphPointer()->AddSplitSecondNode(entity->GetCurrentPlot()->GetGraphPointer()->Peek());
		}
	}
	else if (entity->GetCurrentState() == UOEntity::AIState::react) {
		entity->AddInstantNode(n);
		entity->GetReacts()[0]->AddInstantNode(n);
	}
}