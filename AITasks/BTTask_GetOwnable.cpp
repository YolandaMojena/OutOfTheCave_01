// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_GetOwnable.h"



EBTNodeResult::Type UBTTask_GetOwnable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	const int STRENGTH_TO_WEIGHT = 0.5f;
	const int APPRECIATION_TO_BORROW = 60;
	const int FEAR_TO_BORROW = 70;
	const int RESPECT_TO_BORROW = 80;

	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	//FString arquetype = blackboard->GetValueAsString(blackboard->GetKeyID("Position")); //Deprecated
	//FString arquetype = blackboard->GetValue<UBlackboardKeyType_String>(blackboard->GetKeyID("Arquetype"));
	OntologicFunctions::AffordableUse affordableUse = (OntologicFunctions::AffordableUse) blackboard->GetValue<UBlackboardKeyType_Enum>(blackboard->GetKeyID("AffordableUse"));

	UOEntity* entity = entityController->GetPawn()->FindComponentByClass<UOEntity>();

	vector<UOOwnable*> candidates = entity->GetInventory();
	vector<UOOwnable*> nearbyOwnables = FindNearbyOwnables(entity->GetOwner());
	candidates.insert(candidates.end(), nearbyOwnables.begin(), nearbyOwnables.end());
		
	/*FindNearbyOwnables(entity->GetOwner());
	for (UOOwnable* o : entity->GetInventory()) {
		candidates.push_back(o);
	}*/

	OntologicFunctions ontF;

	UOOwnable* bestChoice = ontF.GetHands();
	int bestChoiceAffordance = ontF.GetAffordance(affordableUse, bestChoice);
	bool bestChoiceSomeoneWhoCares = true;

	for (UOOwnable* ownable : candidates) {
		if (ownable->GetMass() <= entity->GetStrength() * STRENGTH_TO_WEIGHT) {
			bool someoneWhoCares = true; // to not try to possess the object if you already own it or if you are taking it because of HighPriority
			// Consider or not who owns the object if you don't own it depending on if the node is HighPriority
			if (!entity->DoesOwn(ownable) && !blackboard->GetValue<UBlackboardKeyType_Bool>(blackboard->GetKeyID("HighPriority"))) {
				// Search for a friend/dominated/lackey entity to borrow from
				bool borrow = false;
				someoneWhoCares = false;
				for (UOEntity* owner : ownable->GetOwners()) {
					ORelation* relation = owner->GetRelationWith(entity);
					if (relation) {
						someoneWhoCares = true;
						if (relation->GetAppreciation() >= APPRECIATION_TO_BORROW || relation->GetFear() >= FEAR_TO_BORROW || relation->GetRespect() >= RESPECT_TO_BORROW)
							borrow = true;
					}	
				}
				if (someoneWhoCares && !borrow)
					continue;
			}
			int newAffordance = ontF.GetAffordance(affordableUse, ownable);
			if (newAffordance > bestChoiceAffordance) {
				bestChoice = ownable;
				bestChoiceAffordance = newAffordance;
				bestChoiceSomeoneWhoCares = someoneWhoCares;
			}
		}
	}

	if (!bestChoiceSomeoneWhoCares && entity->GetPersonality()->GetMaterialist() > 50) {
		//create ownership!
		entity->AddPossession(bestChoice);
	}

	if (bestChoice != ontF.GetHands()) {
		/*Node* n = new Node();
		n->SetNodeType(NodeType::grab);
		n->SetOwnable(bestChoice);
		entity->AddInstantNode(n);*/
		blackboard->SetValueAsObject(blackboard->GetKeyID("Item"), bestChoice);
	}

	return EBTNodeResult::Succeeded;
}

vector<UOOwnable*> UBTTask_GetOwnable::FindNearbyOwnables(AActor* actor) {
	float const _SEARCH_RADIUS = 300.0f;
	FVector start = actor->GetActorLocation();
	FVector end = start + FVector(0, 0, _SEARCH_RADIUS * 2);
	TArray<FHitResult> hitData;

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, actor);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	actor->GetWorld()->SweepMultiByChannel(
		hitData,
		start,
		end,
		FQuat(),
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(_SEARCH_RADIUS),
		RV_TraceParams
		);

	vector<UOOwnable*> results;
	for (auto hr : hitData) {
		UOOwnable* o = hr.GetActor()->FindComponentByClass<UOOwnable>();
		UOEdification* e = hr.GetActor()->FindComponentByClass<UOEdification>();
		if (!e && o)
			results.push_back(o);
	}
	return results;
}