// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Cultivate.h"


EBTNodeResult::Type UBTTask_Cultivate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	const float GAMEDAY_LENGTH = 60.f;	//s/d
	const float DAY_LENGTH = 24.f;		//h/d

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	if (entity) {
		if (!entity->_isEntityCultivating) {
			float timeToWait = blackboard->GetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("Daytime")) - ((UItem*)entity)->GetPlotGenerator()->dayTime;
			timeToWait *= GAMEDAY_LENGTH / DAY_LENGTH;
			blackboard->SetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("FloatKey"), timeToWait);

			entity->_isEntityCultivating = true;
		}
		else
			entity->_isEntityCultivating = false;
	

		return::EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

