// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "Ontology/EntityAIController.h"
#include "Ontology/OEntity.h"
#include "Ontology/ORelation.h"


#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BTTask_AskForHelpNode.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UBTTask_AskForHelpNode : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintType, EditAnywhere, Category = Path)
		float searchRatio;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
