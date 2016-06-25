// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_GetCombatDistance.h"



EBTNodeResult::Type UBTTask_GetCombatDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityAIController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());


	return EBTNodeResult::Succeeded;
}
