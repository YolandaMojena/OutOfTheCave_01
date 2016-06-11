// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Ontology/EntityAIController.h"
#include "Ontology/OCivilian.h"

#include "BTTask_SetIconPath.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UBTTask_SetIconPath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintType, EditAnywhere, Category = Path)
		FString taskIconPath;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
