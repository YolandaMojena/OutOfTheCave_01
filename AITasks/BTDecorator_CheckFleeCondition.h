// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"

#include "Ontology/EntityAIController.h"
#include "Ontology/OEntity.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "BTDecorator_CheckFleeCondition.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UBTDecorator_CheckFleeCondition : public UBTDecorator
{
	GENERATED_BODY()

protected:
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
