// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_Destroy.h"


EBTNodeResult::Type UBTTask_Destroy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();
	UOEdification* targetEdification = (UOEdification*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Edification"));

	if (entity && targetEdification) {
		blackboard->SetValue<UBlackboardKeyType_Float>(blackboard->GetKeyID("FloatKey"), entity->GetAttackCooldown());

		// APPLY DAMAGE
		OntologicFunctions of = OntologicFunctions();
		float damage = entity->GetStrength() * of.UseAsWeapon(entity->GetGrabbedItem()) / targetEdification->GetToughness();
		targetEdification->ReceiveDamage(damage, entity, GetDestroyPoint(entity->GetOwner()));

		// DESTROYED / WINCON
		if (targetEdification->GetIsDestroyed()) //targetEdification->GetIntegrity() <= UOEntity::MIN_INTEGRITY)
			blackboard->ClearValue(blackboard->GetKeyID("Edification"));
		entity->EndAttack();
		return EBTNodeResult::Succeeded;
	}
	

	return EBTNodeResult::Succeeded;
}

FVector UBTTask_Destroy::GetDestroyPoint(AActor* actor) {
	const int WIDTH = 300;
	const int HEIGHT = 200;

	FHitResult HitData(ForceInit);
	FVector castPoint = actor->GetActorLocation() + actor->GetActorRightVector() * (rand() % WIDTH - WIDTH / 2) + actor->GetActorUpVector() * (rand() % HEIGHT - HEIGHT / 2);

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, actor);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	actor->GetWorld()->LineTraceSingleByChannel(
		HitData,
		castPoint,
		castPoint + actor->GetActorForwardVector()*200,
		ECollisionChannel::ECC_Visibility,
		RV_TraceParams
		);

	return HitData.ImpactPoint;
}
