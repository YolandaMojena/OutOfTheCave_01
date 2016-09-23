// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_DestroySelf.h"


EBTNodeResult::Type UBTTask_DestroySelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	AActor* entityActor = entityController->GetPawn();
	UOEntity* entity = entityActor->FindComponentByClass<UOEntity>();
	

	if (entity) {
		entity->Die();
		entityActor->SetActorLocation(FVector(rand() % 5000, rand() % 5000, -10000));
		//entity->GetOwner()->Destroy();
	}
	return EBTNodeResult::Succeeded;
}

