// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "EntityAIController.h"


AEntityAIController::AEntityAIController() {
	entityBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	behaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}
AEntityAIController::~AEntityAIController() {}


void AEntityAIController::SetNode(Node* n) {
	switch (n->GetNodeType()) {
	case NodeType::askForHelp:
		break;
	case NodeType::attack:
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
		entityBlackboard->SetValue<UBlackboardKeyType_Vector>(entityBlackboard->GetKeyID("Position"), n->nBlackboard.position);
		break;
	case NodeType::grab:
		break;
	case NodeType::interact:
		break;
	case NodeType::steal:
		break;
	case NodeType::wait:
		break;
	default:
		break;
	}
}

void AEntityAIController::Possess(APawn* pawn) {
	AActor* controllerEntity = dynamic_cast<AActor*>(pawn);
	UOEntity* entity = controllerEntity->FindComponentByClass<UOEntity>();

	if (!entity->IsPlayer) {
		Super::Possess(pawn);

		entity->SetAIController(this);
		if (entity && entity->entityBehaviorTree) {
			entityBlackboard->InitializeBlackboard(*entity->entityBehaviorTree->BlackboardAsset);
			nodeTypeID = entityBlackboard->GetKeyID("NodeType");
		}
		behaviorTree->StartTree(*entity->entityBehaviorTree);
	}
}
