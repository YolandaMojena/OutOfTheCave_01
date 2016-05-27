// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OEntityAIController.h"


AOEntityAIController::AOEntityAIController() {

	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	behaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

}

void AOEntityAIController::Possess(APawn* inPawn) {

	Super::Possess(inPawn);

	AEntity* controllerEntity = dynamic_cast<AEntity*>(inPawn);
	if (controllerEntity && controllerEntity->entityBehavior) {

		blackboardComp->InitializeBlackboard(*controllerEntity->entityBehavior->BlackboardAsset);
		targetKeyId = blackboardComp->GetKeyID("target");

		behaviorTreeComp->StartTree(*controllerEntity->entityBehavior);
	}
}
