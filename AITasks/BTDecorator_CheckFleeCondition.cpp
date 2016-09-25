// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTDecorator_CheckFleeCondition.h"


bool UBTDecorator_CheckFleeCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{

	AEntityAIController* entityController = nullptr;
	entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = nullptr;
	blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = nullptr;
	entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOEntity* otherEntity = nullptr;
	otherEntity = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));

	if (entity->IsValidItem() && otherEntity->IsValidItem()) {
		ORelation* relationWithOther = entity->GetRelationWith(otherEntity);
		if (!relationWithOther)
			relationWithOther = entity->AddRelationship(otherEntity);
		if (relationWithOther) {
			if (entity->GetBraveness() + entity->GetPride() + entity->GetIntegrity() <
				relationWithOther->GetFear() + otherEntity->GetIntegrity())
				return true;
		}
	}

	return false;
}

