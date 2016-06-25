// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_AttackMove.h"




EBTNodeResult::Type UBTTask_AttackMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	const int MOVEMENT_RADIUS = 600;
	const int MIN_RADIUS = 400;

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	//FVector entityPosition = entity->GetOwner()->GetActorLocation();
	UOEntity* enemy = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
	FVector enemyPosition = enemy->GetOwner()->GetActorLocation();

	FVector targetPosition = enemyPosition + FVector(rand()-0.5f, rand()-0.5f, 0).GetSafeNormal() * (rand() % (MOVEMENT_RADIUS - MIN_RADIUS) + MIN_RADIUS);
	targetPosition *= FVector(1, 1, 0);

	blackboard->SetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID("Position"), targetPosition);

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, entity->GetName() + TEXT("'s agressivenes: ") + FString::SanitizeFloat(entity->GetAggressiveness()));

	return EBTNodeResult::Succeeded;
}