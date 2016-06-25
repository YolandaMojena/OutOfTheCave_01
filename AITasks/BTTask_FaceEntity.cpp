// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_FaceEntity.h"




EBTNodeResult::Type UBTTask_FaceEntity::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	AActor* entityActor = entity->GetOwner();
	//FVector entityPosition = entity->GetOwner()->GetActorLocation();
	UOEntity* enemy = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
	FVector enemyPosition = enemy->GetOwner()->GetActorLocation();

	//entityActor->SetActorRelativeRotation();

	return EBTNodeResult::InProgress;
}