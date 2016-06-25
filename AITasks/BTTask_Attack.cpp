// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"

#include "BTTask_Attack.h"


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {	
		if (!entity->IsEntityAttacking()) {
			blackboard->SetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("FloatKey"), entity->GetAttackCooldown());
			return EBTNodeResult::Succeeded;
		}
		/*else
			return EBTNodeResult::InProgress;*/
	}
	

	return EBTNodeResult::Failed;
}

