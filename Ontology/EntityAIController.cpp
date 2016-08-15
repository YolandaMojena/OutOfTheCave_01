// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "EntityAIController.h"


AEntityAIController::AEntityAIController() {
	entityBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	behaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}
AEntityAIController::~AEntityAIController() {}



void AEntityAIController::SetNode(Node* n) {

	entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(n->GetNodeType()));
	entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("Daytime"), n->nBlackboard.daytime);
	switch (n->GetNodeType()) {
	case NodeType::askForHelp:
		break;
	case NodeType::attack:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity") , n->nBlackboard.entity);
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.entity->GetOwner());
		entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("FloatKey"), n->nBlackboard.floatKey);
		break;
	case NodeType::branch:
		break;
	case NodeType::build:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Edification"), n->nBlackboard.edification);
		break;
	case NodeType::celebrate:
		break;
	case NodeType::destroy:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Edification"), n->nBlackboard.edification);
		break;
	case NodeType::enter:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Residence"), (UOResidence*) n->nBlackboard.edification);
		break;
	case NodeType::get:
		//entityBlackboard->SetValue<UBlackboardKeyType_Enum>(entityBlackboard->GetKeyID("AffordableUse"), static_cast<UBlackboardKeyType_Enum::FDataType>(n->nBlackboard.affordableUse));
	{
		//TestBegin
		//UOEntity* entity = GetControlledPawn()->FindComponentByClass<UOEntity>();
		UOEntity* entity = GetPawn()->FindComponentByClass<UOEntity>();
		UOOwnable* itemToGrab = GetOwnable(entity, n->nBlackboard.affordableUse, n->nBlackboard.isHighPriority);
		if (itemToGrab) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Item"), (UItem*)itemToGrab);
			if (itemToGrab->GetOwner())
				entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), itemToGrab->GetOwner());

			if (entity->HasGrabbedItem()) {
				if ((entity->GetGrabbedItem()->GetOwner()->GetActorLocation() - entity->GetHome()->GetOwner()->GetActorLocation()).Size() > (entity->GetOwner()->GetActorLocation() - entity->GetHome()->GetOwner()->GetActorLocation()).Size()) {
					entity->ReleaseGrabbedItem();
				}
			}

			entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(NodeType::grab));
		}
		//TestEnd
	}
		break;
	case NodeType::give:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
		break;
	case NodeType::goTo:
		entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, n->nBlackboard.position  * FVector(1, 1, 0));
		break;
	case NodeType::goToActor:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		break;
	case NodeType::grab:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		break;
	case NodeType::cultivate:
		break;
	case NodeType::mine:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		break;
	case NodeType::steal:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
		break;
	case NodeType::wait:
		if(n->nBlackboard.floatKey > 0)
			entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("FloatKey"), n->nBlackboard.floatKey);
		else
			entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("FloatKey"), 5);
		break;
	case NodeType::waitUntilDaytime:
		break;
	case NodeType::help:
		entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
		if(n->nBlackboard.entity->IsPlayer)
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		break;
	case NodeType::preyUpon:	// Convertible Node
	{
		UOEntity* prey = GetOwner()->FindComponentByClass<UOEntity>()->FindPrey();
		if (prey) {
			entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(NodeType::attack));
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), prey);
		}
		/*else {
			entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(NodeType::goTo));
			entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, GetOwner()->GetActorLocation() + Utilities::RandomDisplacementVector(1000));
		}*/
	}
	break;
	default:
		break;
	}
}



void AEntityAIController::SetState(UOEntity::AIState s) {
	
	entityBlackboard->SetValue<UBlackboardKeyType_Enum>(entityBlackboard->GetKeyID("EntityState"), static_cast<UBlackboardKeyType_Enum::FDataType>(s));
	if (s == UOEntity::AIState::numb) entityBlackboard->ClearValue(nodeTypeID);
}



void AEntityAIController::Possess(APawn* pawn) {
	AActor* controllerEntity = dynamic_cast<AActor*>(pawn);
	UOEntity* entity = controllerEntity->FindComponentByClass<UOEntity>();

	if (entity && !entity->IsPlayer) {
		Super::Possess(pawn);

		if (entity->entityBehaviorTree) {
			entity->SetAIController(this);
			entityBlackboard->InitializeBlackboard(*entity->entityBehaviorTree->BlackboardAsset);
			nodeTypeID = entityBlackboard->GetKeyID("NodeType");
			positionID = entityBlackboard->GetKeyID("Position");


			//behaviorTree->StartTree(*entity->entityBehaviorTree);
			RunBehaviorTree(entity->entityBehaviorTree);

			// Default value is true
			entityBlackboard->SetValue<UBlackboardKeyType_Bool>(entityBlackboard->GetKeyID("CompletedOk"), true);
		}
	}
}






//			A U X
void AEntityAIController::TryLeaveGrabbedObject(UOEntity* entity) {
	
}









UOOwnable* AEntityAIController::GetOwnable(UOEntity* entity, OntologicFunctions::AffordableUse affordableUse, bool isHighPriority) {

	const int STRENGTH_TO_WEIGHT = 2;
	const int APPRECIATION_TO_BORROW = 60;
	const int FEAR_TO_BORROW = 70;
	const int RESPECT_TO_BORROW = 80;


	//AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	//UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();


	vector<UOOwnable*> candidates = FindNearbyOwnables(entity);
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, TEXT("NearbyOwnables: ") + FString::SanitizeFloat(candidates.size()));

	OntologicFunctions ontF;

	UOOwnable* bestChoice = ontF.GetHands();
	int bestChoiceAffordance;
	bestChoiceAffordance = ontF.GetAffordance(affordableUse, bestChoice);
	bool bestChoiceSomeoneWhoCares = true;

	if (entity->HasGrabbedItem()) {
		UOOwnable* grabbedItem = (UOOwnable*)entity->GetGrabbedItem();
		int grabbedItemAffordance = ontF.GetAffordance(affordableUse, grabbedItem);
		if (grabbedItemAffordance > bestChoiceAffordance) {
			bestChoice = grabbedItem;
		}
	}

	for (UOOwnable* ownable : candidates) {
		if (ownable->GetMass() <= entity->GetStrength() / STRENGTH_TO_WEIGHT && !ownable->GetIsGrabbed()) {
			bool someoneWhoCares = true; // to not try to possess the object if you already own it or if you are taking it because of HighPriority
										 // Consider or not who owns the object if you don't own it depending on if the node is HighPriority
			if (!entity->DoesOwn(ownable) && !isHighPriority) {
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
				if (someoneWhoCares && !borrow) {
					continue;
				}
			}
			int newAffordance = ontF.GetAffordance(affordableUse, ownable);
			GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, TEXT("BCA: ") + FString::SanitizeFloat(bestChoiceAffordance));
			GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, TEXT("NA: ") + FString::SanitizeFloat(newAffordance));
			if (newAffordance > bestChoiceAffordance) {
				bestChoice = ownable;
				bestChoiceAffordance = newAffordance;
				bestChoiceSomeoneWhoCares = someoneWhoCares;
				GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, TEXT("Better one!"));
				GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, TEXT("BCA: ") + FString::SanitizeFloat(bestChoiceAffordance));
			}
		}
	}

	if (!bestChoiceSomeoneWhoCares && entity->GetPersonality()->GetMaterialist() > 50) {
		//create ownership!
		entity->AddPossession(bestChoice);
	}

	if (bestChoice != ontF.GetHands() && bestChoice != entity->GetGrabbedItem())
	{
		//blackboard->SetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Item"), (UItem*)bestChoice);
		//if (bestChoice->GetOwner())
		//	blackboard->SetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Actor"), bestChoice->GetOwner());

		bestChoice->AddGrabber(entity);
		return bestChoice;
	}

	//return bestChoice;
	return nullptr;
}

vector<UOOwnable*> AEntityAIController::FindNearbyOwnables(UOEntity* entity) {
	AActor* actor = entity->GetOwner();
	float const _SEARCH_RADIUS = 3000.0f;
	FVector start = actor->GetActorLocation() + FVector(0, 0, -_SEARCH_RADIUS);
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
		UOEdification* edf = hr.GetActor()->FindComponentByClass<UOEdification>();
		if (!edf && o) {
			bool admit = true;
			vector<UOEntity*> grabbers = o->GetGrabbers();
			vector<ORelation*> relatives = entity->GetRelationships();
			for (ORelation* r : relatives) {
				UOEntity* e = r->GetOtherEntity();
				for (UOEntity* g : grabbers) {
					if (e == g && (r->GetAppreciation() > 25 || r->GetRespect() > 37 || r->GetFear() > 25)) {
						admit = false;
						break;
					}
				}
				if (!admit)
					break;
			}

			if (admit) {
				for (UOOwnable* own : results) {
					if (o == own) {
						admit = false;
						break;
					}
				}
				if (admit)
					results.push_back(o);
			}
		}
	}
	return results;
}