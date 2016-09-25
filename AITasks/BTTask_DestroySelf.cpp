// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_DestroySelf.h"


EBTNodeResult::Type UBTTask_DestroySelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = nullptr;
	entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = nullptr;
	blackboard = OwnerComp.GetBlackboardComponent();

	AActor* entityActor = nullptr;
	entityActor = entityController->GetPawn();
	UOEntity* entity = nullptr;
	entity = entityActor->FindComponentByClass<UOEntity>();
	

	if (entity) {
		entity->Die();
		entity->GetOwner()->SetActorHiddenInGame(true);
		entity->GetOwner()->SetActorEnableCollision(false);
		entityActor->SetActorLocation(FVector(rand() % 5000, rand() % 5000, -10000));
		//entity->GetOwner()->Destroy();
	}
	return EBTNodeResult::Succeeded;
}

