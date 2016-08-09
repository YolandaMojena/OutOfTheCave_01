// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Wreck.h"


EBTNodeResult::Type UBTTask_Wreck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOEdification* targetEdification = (UOEdification*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Edification"));

	if (entity && targetEdification) {
		blackboard->SetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("FloatKey"), entity->GetAttackCooldown());

		// APPLY DAMAGE
		OntologicFunctions of = OntologicFunctions();
		float damage = entity->GetStrength() * of.UseAsWeapon(entity->GetGrabbedItem()) / targetEdification->GetToughness();
		targetEdification->ReceiveDamage(damage, entity);

		// DESTROYED / WINCON
		if (targetEdification->GetIntegrity() <= UOEntity::MIN_INTEGRITY)
			blackboard->ClearValue(blackboard->GetKeyID("Edification"));
		entity->EndAttack();
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}

