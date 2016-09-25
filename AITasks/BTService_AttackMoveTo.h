// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "Ontology/EntityAIController.h"
#include "Ontology/OEntity.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "BTService_AttackMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UBTService_AttackMoveTo : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void AbortCombat(UBlackboardComponent* blackboard);

	void ChangeToFlee(UOEntity* entity, AActor* horrorActor);
};
