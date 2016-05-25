// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_MoveToTaskTest.h"

EBTNodeResult::Type UBTTask_MoveToTaskTest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AOEntityAIController* ownerEntityController = dynamic_cast<AOEntityAIController*>(OwnerComp.GetAIOwner());

	if (ownerEntityController) {

		ATroll* target = dynamic_cast<ATroll*>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(ownerEntityController->targetKeyId));

		if (target) {

			if (target->GetActorLocation().Dist(target->GetActorLocation(), ownerEntityController->GetPawn()->GetActorLocation()) < 500.0f)
			{
				//ownerEntityController->MoveToActor(target, 10, true, true, true, 0, true);
				return EBTNodeResult::Succeeded;
			}
			else return EBTNodeResult::Failed;
		}
		else return EBTNodeResult::Failed;
	}
	else return EBTNodeResult::Failed;
}


