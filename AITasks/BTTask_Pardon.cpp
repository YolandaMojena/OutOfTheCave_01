// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Pardon.h"


EBTNodeResult::Type UBTTask_Pardon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	const int DISTANCE = 600;

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	FVector entityPosition = entity->GetOwner()->GetActorLocation();
	UOEntity* enemy = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
	FVector enemyPosition = enemy->GetOwner()->GetActorLocation();

	FVector targetPosition = entityPosition + (entityPosition - enemyPosition).GetSafeNormal() * DISTANCE;
	targetPosition *= FVector(1, 1, 0);

	blackboard->SetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID("Position"), targetPosition);
	blackboard->ClearValue(blackboard->GetKeyID("Entity"));

	return EBTNodeResult::Succeeded;
}
