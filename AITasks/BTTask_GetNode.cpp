// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_GetNode.h"



EBTNodeResult::Type UBTTask_GetNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	//FString arquetype = blackboard->GetValueAsString(blackboard->GetKeyID("Position")); //Deprecated
	//FString arquetype = blackboard->GetValue<UBlackboardKeyType_String>(blackboard->GetKeyID("Arquetype"));
	OntologicFunctions::AffordableUse affordableUse = (OntologicFunctions::AffordableUse) blackboard->GetValue<UBlackboardKeyType_Enum>(blackboard->GetKeyID("AffordableUse"));

	return EBTNodeResult::Succeeded;
}
