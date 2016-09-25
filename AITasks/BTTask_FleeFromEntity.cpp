// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_FleeFromEntity.h"




EBTNodeResult::Type UBTTask_FleeFromEntity::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	const int BASE_FLEE_DISTANCE = 500;
	const int BASE_FLEE_MULTIPLIER = 4;

	AEntityAIController* entityController = nullptr;
	entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = nullptr;
	blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = nullptr;
	entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	FVector entityPosition = entity->GetOwner()->GetActorLocation();
	UOEntity* horror = nullptr;
	horror = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
	FVector horrorPosition = horror->GetOwner()->GetActorLocation();

	if (entity) {

		FVector targetPosition = entityPosition + (entityPosition - horrorPosition).GetSafeNormal() * (BASE_FLEE_DISTANCE + (100 - entity->GetPersonality()->GetBraveness() + entity->GetRelationWith(horror)->GetFear()) * BASE_FLEE_MULTIPLIER);
		targetPosition *= FVector(1, 1, 0);

		blackboard->SetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID("Position"), targetPosition);
		//blackboard->SetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"), nullptr);
		blackboard->ClearValue(blackboard->GetKeyID("Entity"));

		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}