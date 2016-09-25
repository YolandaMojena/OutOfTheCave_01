// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Mine.h"


EBTNodeResult::Type UBTTask_Mine::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = nullptr;
	entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {
		if (!entity->_isEntityMining) {
			float timeToWait = blackboard->GetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("Daytime")) - ((UItem*)entity)->GetPlotGenerator()->dayTime;
			timeToWait *= Utilities::GAMEDAY_LENGTH / Utilities::DAY_LENGTH;
			blackboard->SetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("FloatKey"), timeToWait);

			entity->_isEntityMining = true;
		}
		else 
			entity->_isEntityMining = false;
	

		return::EBTNodeResult::Succeeded;
	}

	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}



