// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Node.h"
#include "Ontology/Item.h"
#include "Ontology/OEntity.h"
#include "Ontology/OEdification.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OResidence.h"
#include "Ontology/OntologicFunctions.h"

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


public:

	UPROPERTY(transient)
	class UBlackboardComponent* entityBlackboard;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* behaviorTree;


	AEntityAIController();
	~AEntityAIController();

	virtual void Possess(APawn* pawn) override;

	void SetNode(Node* n);
	void SetState(UOEntity::AIState s);
	//void ExecuteNode();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityAIController)
	uint8 nodeTypeID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EntityAIController)
	uint8 positionID;

private:
	Node* _currentNode;
	
	void TryLeaveGrabbedObject(UOEntity* entity);

	UOOwnable* GetOwnable(UOEntity* entity, OntologicFunctions::AffordableUse affordableUse, bool isHighPriority);
	vector<UOOwnable*> FindNearbyOwnables(UOEntity* entity);

	FVector RandomDisplacementVector(int radius);

	void ReleaseGrabbedItemNearestToHome(UOEntity* entity, FVector targetPosition);

	void AbortNode();

	bool ValidateObject(UObject* obj);
};
