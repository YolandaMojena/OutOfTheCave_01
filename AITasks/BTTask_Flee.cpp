// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Flee.h"



EBTNodeResult::Type UBTTask_Flee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = nullptr;
	entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = nullptr;
	blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = nullptr;
	entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	FVector entityPosition = entity->GetOwner()->GetActorLocation();
	AActor* horror = nullptr;
	horror = (AActor*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Actor"));

	if (entity && horror) {
		FVector horrorPosition = horror->GetActorLocation();
		
		if ((entityPosition - horrorPosition).Size() > BASE_SECURE_DISTANCE)
		{
			blackboard->ClearValue(blackboard->GetKeyID("Actor"));
		}
		else {

			FVector targetPosition = entityPosition + (entityPosition - horrorPosition).GetSafeNormal() * BASE_FLEE_DISTANCE;
			targetPosition *= FVector(1, 1, 0);
			blackboard->SetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID("Position"), targetPosition);
		}
		EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}
