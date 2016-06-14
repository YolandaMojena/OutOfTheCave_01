// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "EntityAIController.h"


AEntityAIController::AEntityAIController() {
	entityBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	behaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}
AEntityAIController::~AEntityAIController() {}


void AEntityAIController::SetNode(Node* n) {
	entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(n->GetNodeType()));

	switch (n->GetNodeType()) {
	case NodeType::askForHelp:
		break;
	case NodeType::attack:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("EntityA") , n->nBlackboard.entityA);
		break;
	case NodeType::branch:
		break;
	case NodeType::build:
		break;
	case NodeType::celebrate:
		break;
	case NodeType::destroy:
		break;
	case NodeType::enter:
		break;
	case NodeType::get:
		break;
	case NodeType::give:
		break;
	case NodeType::goTo:
		entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, n->nBlackboard.position);
		break;
	case NodeType::goToItem:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("ActorA"), n->nBlackboard.actorA);
	case NodeType::grab:
		break;
	case NodeType::cultivate:
		break;
	case NodeType::steal:
		break;
	case NodeType::wait:
		break;
	default:
		break;
	}
}

void AEntityAIController::SetState(UOEntity::State s) {
	entityBlackboard->SetValue<UBlackboardKeyType_Enum>(entityBlackboard->GetKeyID("EntityState"), static_cast<UBlackboardKeyType_Enum::FDataType>(s));
}

void AEntityAIController::Possess(APawn* pawn) {
	AActor* controllerEntity = dynamic_cast<AActor*>(pawn);
	UOEntity* entity = controllerEntity->FindComponentByClass<UOEntity>();

	if (entity &&  !entity->IsPlayer) {
		Super::Possess(pawn);

		if (entity->entityBehaviorTree) {
			entity->SetAIController(this);
			entityBlackboard->InitializeBlackboard(*entity->entityBehaviorTree->BlackboardAsset);
			nodeTypeID = entityBlackboard->GetKeyID("NodeType");
			positionID = entityBlackboard->GetKeyID("Position");


			//behaviorTree->StartTree(*entity->entityBehaviorTree);
			RunBehaviorTree(entity->entityBehaviorTree);

			// Default value is true
			entityBlackboard->SetValue<UBlackboardKeyType_Bool>(entityBlackboard->GetKeyID("CompletedOk"), true);
		}
	}
}
