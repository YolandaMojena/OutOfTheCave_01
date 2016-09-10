// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"

#include "BTTask_Attack.h"


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOEntity* targetEntity = (UOEntity*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Entity"));

	if (entity && targetEntity) {

		if (targetEntity->GetIntegrity() <= UOEntity::MIN_INTEGRITY) {
			blackboard->ClearValue(blackboard->GetKeyID("Entity"));
			entity->EndAttack();
			return EBTNodeResult::Succeeded;
		}


		blackboard->SetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("FloatKey"), entity->GetAttackCooldown());
		
		// APPLY DAMAGE
		if ((targetEntity->GetOwner()->GetActorLocation() - entity->GetOwner()->GetActorLocation()).Size() < entity->GetGrabbedItem()->GetFuncPos().Size() + 200.f) {
			//targetEntity->ReceiveDamage(25 + rand() % 50, entity);
			OntologicFunctions of = OntologicFunctions();
			float damage = entity->GetStrength() * of.UseAsWeapon(entity->GetGrabbedItem()) / targetEntity->GetToughness();
			targetEntity->ReceiveDamage(damage, entity);
		}

		// DEATH / WINCON
		if (targetEntity->GetIntegrity() <= UOEntity::MIN_INTEGRITY)
			blackboard->ClearValue(blackboard->GetKeyID("Entity"));
		entity->EndAttack();
		return EBTNodeResult::Succeeded;

		/*if (!entity->IsEntityAttacking()) {
			blackboard->SetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("FloatKey"), entity->GetAttackCooldown());
			return EBTNodeResult::Succeeded;
		}
		else {
			return EBTNodeResult::InProgress;
		}*/
	}
	
	blackboard->SetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("CompletedOk"), false);
	return EBTNodeResult::Succeeded;
}

