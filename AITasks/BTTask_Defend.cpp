// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Defend.h"

EBTNodeResult::Type UBTTask_Defend::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {

		if ((((AActor*)blackboard->GetValueAsObject(blackboard->GetKeyID("Actor")))->GetActorLocation() - entity->GetOwner()->GetActorLocation()).Size() <= tolerance) {
			blackboard->SetValue<UBlackboardKeyType_Bool>("BoolKey", true);
			blackboard->SetValue<UBlackboardKeyType_Float>("FloatKey", 0);
		}
		else
			blackboard->SetValue<UBlackboardKeyType_Float>("FloatKey", blackboard->GetValueAsFloat(blackboard->GetKeyID("FloatKey"))-1);
	}

	return::EBTNodeResult::Succeeded;
}


