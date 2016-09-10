// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTDecorator_CheckFleeCondition.h"


bool UBTDecorator_CheckFleeCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOEntity* otherEntity = (UOEntity*) blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));

	if (entity && otherEntity) {
		ORelation* relationWithOther = entity->GetRelationWith(otherEntity);
		if (!relationWithOther)
			relationWithOther = entity->AddRelationship(otherEntity);
		if (entity->GetBraveness() + entity->GetPride() + entity->GetIntegrity() <
			relationWithOther->GetFear() + otherEntity->GetIntegrity())
			return true;
	}

	return false;
}

