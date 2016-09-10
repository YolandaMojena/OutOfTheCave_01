// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTDecorator_CheckPardonCondition.h"




bool UBTDecorator_CheckPardonCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOEntity* otherEntity = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));

	if (entity && otherEntity) {
		if (otherEntity->GetBrain()->Peek()->GetNodeType() == NodeType::flee) {
			ORelation* relationWithOther = entity->GetRelationWith(otherEntity);
			if (!relationWithOther)
				relationWithOther = entity->AddRelationship(otherEntity);
			if (otherEntity->GetIntegrity() < relationWithOther->GetAppreciation() / 2 + entity->GetKindness() / 2 + entity->GetIntegrity() - entity->GetAggressiveness())
				return true;
		}
		
		if (FVector::Dist(entity->GetOwner()->GetActorLocation(), otherEntity->GetOwner()->GetActorLocation()) > 2000) {
			ORelation* relationWithOther = entity->GetRelationWith(otherEntity);
			if (!relationWithOther)
				relationWithOther = entity->AddRelationship(otherEntity);
			if(relationWithOther->GetAppreciation() > 5)
				return true;
		}
	}
	return false;
}