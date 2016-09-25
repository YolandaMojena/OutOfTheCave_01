// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Steal.h"


EBTNodeResult::Type UBTTask_Steal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {


	AEntityAIController* entityController = nullptr;
	entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = nullptr;
	blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = nullptr;
	entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	
	if (entity) {

		UOEntity* targetEntity = nullptr;
		targetEntity = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));
		UOOwnable* targetOwnable = nullptr;
		targetOwnable = (UOOwnable*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Ownable"));

		if (targetEntity && targetOwnable) {
			entity->GrabItem(targetOwnable);
			if(targetEntity->GetOwnershipWith(targetOwnable))
				targetOwnable->IHaveBeenStolenBySomeone(targetEntity, entity);
		}
		return EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}

