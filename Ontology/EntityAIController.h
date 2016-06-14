// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Node.h"
#include "Ontology/OEntity.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "EntityAIController.generated.h"

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API AEntityAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(transient)
	class UBlackboardComponent* entityBlackboard;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* behaviorTree;

public:
	AEntityAIController();
	~AEntityAIController();

	virtual void Possess(APawn* pawn) override;

	void SetNode(Node* n);
	void SetState(UOEntity::State s);
	void ExecuteNode();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityAIController)
	uint8 nodeTypeID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityAIController)
	uint8 positionID;

private:
	Node* _currentNode;

	
	
};
