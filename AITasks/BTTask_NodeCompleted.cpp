// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_NodeCompleted.h"

EBTNodeResult::Type UBTTask_NodeCompleted::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass <UOEntity>();

	if (entity) {
		entity->SetLastNode(entity->GetBrain()->Peek());
		DidIHelpSomeone(entity, blackboard);
		ClearBlackboard(blackboard);
		entity->NodeCompleted(blackboard->GetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk")));
		blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), true);
	}
	return EBTNodeResult::Succeeded;
}

void UBTTask_NodeCompleted::ClearBlackboard(UBlackboardComponent* blackboard) {

	blackboard->ClearValue(blackboard->GetKeyID("Actor"));
	blackboard->ClearValue(blackboard->GetKeyID("Item"));
	blackboard->ClearValue(blackboard->GetKeyID("Ownable"));
	blackboard->ClearValue(blackboard->GetKeyID("Edification"));
	blackboard->ClearValue(blackboard->GetKeyID("Entity"));
	blackboard->ClearValue(blackboard->GetKeyID("AffordableUse"));
	blackboard->ClearValue(blackboard->GetKeyID("Position"));
	blackboard->ClearValue(blackboard->GetKeyID("BoolKey"));
	blackboard->ClearValue(blackboard->GetKeyID("FloatKey"));
	blackboard->ClearValue(blackboard->GetKeyID("Residence"));
	
}

void UBTTask_NodeCompleted::DidIHelpSomeone(UOEntity* me, UBlackboardComponent* blackboard) {
	for (UOEntity* other : me->GetNearbyEntities()) {
		Node* myNode = me->GetBrain()->Peek();
		Node* otherNode = other->GetBrain()->Peek();
		Node* otherLastNode = other->GetLastNode();

		bool helping = false;
		if (myNode) {
			if (otherNode) {
				if (myNode->GetNodeType() == otherNode->GetNodeType()) {
					if (myNode->nBlackboard.actor && otherNode->nBlackboard.actor &&
						myNode->nBlackboard.actor == otherNode->nBlackboard.actor) {
						helping = true;
					}
					else if (myNode->nBlackboard.affordableUse && otherNode->nBlackboard.affordableUse &&
							myNode->nBlackboard.affordableUse == otherNode->nBlackboard.affordableUse) {
						helping = true;
					}
					else if (myNode->nBlackboard.edification && otherNode->nBlackboard.edification &&
							myNode->nBlackboard.edification == otherNode->nBlackboard.edification) {
						helping = true;
					}
					else if (myNode->nBlackboard.entity && otherNode->nBlackboard.entity &&
							myNode->nBlackboard.entity == otherNode->nBlackboard.entity) {
						helping = true;
					}
					/*else if (myNode->nBlackboard.ownable && otherNode->nBlackboard.ownable && 
							myNode->nBlackboard.ownable == otherNode->nBlackboard.ownable) {
						helping = true;
					}*/
				}
			}
			if (!helping && otherLastNode) {
				if (myNode->GetNodeType() == otherLastNode->GetNodeType()) {
					if (myNode->nBlackboard.actor && otherLastNode->nBlackboard.actor &&
						myNode->nBlackboard.actor == otherLastNode->nBlackboard.actor) {
						helping = true;
					}
					else if (myNode->nBlackboard.affordableUse && otherLastNode->nBlackboard.affordableUse &&
						myNode->nBlackboard.affordableUse == otherLastNode->nBlackboard.affordableUse) {
						helping = true;
					}
					else if (myNode->nBlackboard.edification && otherLastNode->nBlackboard.edification &&
						myNode->nBlackboard.edification == otherLastNode->nBlackboard.edification) {
						helping = true;
					}
					else if (myNode->nBlackboard.entity && otherLastNode->nBlackboard.entity &&
						myNode->nBlackboard.entity == otherLastNode->nBlackboard.entity) {
						helping = true;
					}
					/*else if (myNode->nBlackboard.ownable && otherLastNode->nBlackboard.ownable &&
					myNode->nBlackboard.ownable == otherLastNode->nBlackboard.ownable) {
					helping = true;
					}*/
				}
			}
		}
		if (helping) {
			ORelation* r = other->GetRelationWith(me);
			if (!r) {
				r = other->AddRelationship(me);
			}
			r->ChangeAppreciation(UOEntity::HELP_APPRECIATION_INCREASE);
			r->ChangeFear(UOEntity::HELP_FEAR_DECREASE);
		}
	}
}
