// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Steal.h"


EBTNodeResult::Type UBTTask_Steal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {


	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	
	if (entity) {

		UOEntity* targetEntity = (UOEntity*) blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
		UOOwnable* targetOwnable = (UOOwnable*) blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Ownable"));

		if (targetEntity && targetOwnable) {

			/*if(targetEntity->StealFromInventory(targetOwnable, targetEntity))
				return EBTNodeResult::Succeeded;
			else return EBTNodeResult::Failed;*/
		}
	}
	return EBTNodeResult::Succeeded;
}

