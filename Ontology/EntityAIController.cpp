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
	entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("Daytime"), n->nBlackboard.daytime);
	switch (n->GetNodeType()) {
	case NodeType::askForHelp:
		break;
	case NodeType::attack:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity") , n->nBlackboard.entity);
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.entity->GetOwner());
		entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("FloatKey"), n->nBlackboard.floatKey);
		break;
	case NodeType::branch:
		break;
	case NodeType::build:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Edification"), n->nBlackboard.edification);
		break;
	case NodeType::celebrate:
		break;
	case NodeType::destroy:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Edification"), n->nBlackboard.edification);
		break;
	case NodeType::enter:
		break;
	case NodeType::get:
		entityBlackboard->SetValue<UBlackboardKeyType_Enum>(entityBlackboard->GetKeyID("AffordableUse"), static_cast<UBlackboardKeyType_Enum::FDataType>(n->nBlackboard.affordableUse));
		break;
	case NodeType::give:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
		break;
	case NodeType::goTo:
		entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, n->nBlackboard.position  * FVector(1, 1, 0));
		break;
	case NodeType::goToItem:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		break;
	case NodeType::grab:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		break;
	case NodeType::cultivate:
		break;
	case NodeType::mine:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		break;
	case NodeType::steal:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
		break;
	case NodeType::wait:
		break;
	case NodeType::help:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
		if(n->nBlackboard.entity->IsPlayer)
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		break;
	default:
		break;
	}
}

void AEntityAIController::SetState(UOEntity::State s) {
	
	entityBlackboard->SetValue<UBlackboardKeyType_Enum>(entityBlackboard->GetKeyID("EntityState"), static_cast<UBlackboardKeyType_Enum::FDataType>(s));
	if (s == UOEntity::State::numb) entityBlackboard->ClearValue(nodeTypeID);
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
