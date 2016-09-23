// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "EntityAIController.h"


AEntityAIController::AEntityAIController() {
	entityBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	behaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}
AEntityAIController::~AEntityAIController() {}



void AEntityAIController::SetNode(Node* n) {

	if (!n)
		return;

	entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(n->GetNodeType()));
	if(n->nBlackboard.daytime)
		entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("Daytime"), n->nBlackboard.daytime);
	
	switch (n->GetNodeType()) {
	case NodeType::askForHelp:
		break;
	case NodeType::askTroll:
		if(ValidateObject(n->nBlackboard.actor))
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		else
			AbortNode();
		break;
	case NodeType::attack:
	{
		if (ValidateObject(n->nBlackboard.entity)) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.entity->GetOwner());
			UOEntity* entity = GetPawn()->FindComponentByClass<UOEntity>();
			if(entity)
				entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("FloatKey"), entity->GetAttackCooldown());
			else
				entityBlackboard->SetValue<UBlackboardKeyType_Float>(entityBlackboard->GetKeyID("FloatKey"), 1.f);
		}
		else
			AbortNode();
	}
		break;
	case NodeType::build:
		if (ValidateObject(n->nBlackboard.edification)) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Edification"), n->nBlackboard.edification);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.edification->GetOwner());
		}
		else
			AbortNode();
		break;
	//case NodeType::celebrate:
	//	break;
	case NodeType::destroy:
		if (ValidateObject(n->nBlackboard.edification)) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Edification"), n->nBlackboard.edification);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.edification->GetOwner());
		}
		else
			AbortNode();
		break;
	case NodeType::enter:
		if (ValidateObject(n->nBlackboard.edification)) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Residence"), (UOResidence*)n->nBlackboard.edification);
			entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, n->nBlackboard.edification->GetOwner()->GetActorLocation() * FVector(1, 1, 0));
		}
		else
			AbortNode();
		break;
	case NodeType::get:
		//entityBlackboard->SetValue<UBlackboardKeyType_Enum>(entityBlackboard->GetKeyID("AffordableUse"), static_cast<UBlackboardKeyType_Enum::FDataType>(n->nBlackboard.affordableUse));
	{
		//TestBegin
		//UOEntity* entity = GetControlledPawn()->FindComponentByClass<UOEntity>();
		UOEntity* entity = GetPawn()->FindComponentByClass<UOEntity>();
		if (ValidateObject(entity) && entity->IsA<UOCivilian>() && n->nBlackboard.affordableUse) {
			UOOwnable* itemToGrab = GetOwnable(entity, n->nBlackboard.affordableUse, n->nBlackboard.isHighPriority);
			if (ValidateObject(itemToGrab)) {
				//entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Item"), (UItem*)itemToGrab);
				entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), itemToGrab);
				if (ValidateObject(itemToGrab->GetOwner()))
					entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), itemToGrab->GetOwner());

				// Release currently grabbed item
				ReleaseGrabbedItemNearestToHome(entity, itemToGrab->GetOwner()->GetActorLocation());
				/*if (entity->HasGrabbedItem()) {
					if ((itemToGrab->GetOwner()->GetActorLocation() - entity->GetHome()->GetOwner()->GetActorLocation()).Size() > (entity->GetOwner()->GetActorLocation() - entity->GetHome()->GetOwner()->GetActorLocation()).Size()) {
						entity->ReleaseGrabbedItem();
					}
				}*/
				//n->SetNodeType(NodeType::grab);
				entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(NodeType::grab));	
			}
		}
		else
			AbortNode();
		//TestEnd
	}
		break;
	case NodeType::give:
		if (ValidateObject(n->nBlackboard.ownable) && ValidateObject(n->nBlackboard.entity)) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.entity->GetOwner());
		}
		else
			AbortNode();
		break;
	case NodeType::goTo:
		entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, n->nBlackboard.position * FVector(1, 1, 0));
		break;
	case NodeType::goToActor:
		if(ValidateObject(n->nBlackboard.actor))
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		else
			AbortNode();
		break;
	case NodeType::defend:
		if (ValidateObject(n->nBlackboard.entity) && ValidateObject(n->nBlackboard.entity->GetOwner())) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.entity->GetOwner());
			//entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		}
		else
			AbortNode();
		break;
	case NodeType::grab:
	{
		UOCivilian* civilian = GetPawn()->FindComponentByClass<UOCivilian>();
		if (ValidateObject(civilian) && ValidateObject(n->nBlackboard.ownable) && ValidateObject(n->nBlackboard.ownable->GetOwner())) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
			//entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.ownable->GetOwner());
			ReleaseGrabbedItemNearestToHome(civilian, n->nBlackboard.ownable->GetOwner()->GetActorLocation());
		}
		else
			AbortNode();
	}
		break;
	case NodeType::cultivate:
		if(ValidateObject(n->nBlackboard.edification) && ValidateObject(n->nBlackboard.edification->GetOwner()))
			entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, (n->nBlackboard.edification->GetOwner()->GetActorLocation() + RandomDisplacementVector(400)) * FVector(1, 1, 0));
		else
			AbortNode();
		break;
	case NodeType::mine:
		if(ValidateObject(n->nBlackboard.actor))
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		else
			AbortNode();
		break;
	case NodeType::steal:
		if (ValidateObject(n->nBlackboard.ownable) && ValidateObject(n->nBlackboard.entity) && ValidateObject(n->nBlackboard.entity->GetOwner())) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Ownable"), n->nBlackboard.ownable);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.entity->GetOwner());
			ReleaseGrabbedItemNearestToHome(n->nBlackboard.entity, n->nBlackboard.ownable->GetOwner()->GetActorLocation());
		}
		else
			AbortNode();
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
		if (ValidateObject(n->nBlackboard.entity)) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
			if (n->nBlackboard.entity->IsPlayer)
				entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.entity->GetOwner());
			//entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
			entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, n->nBlackboard.position  * FVector(1, 1, 0));
		}
		else
			AbortNode();
		break;
	case NodeType::preyUpon:	// Convertible Node
	{
		UOEntity* entity = GetPawn()->FindComponentByClass<UOEntity>();
		if (ValidateObject(entity)) {
			UOEntity* prey = entity->FindPrey();
			if (ValidateObject(prey)) {
				entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(NodeType::attack));
				entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), prey);
			}
			else
				AbortNode();
		}
		else
			AbortNode();
		/*else {
			entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(NodeType::goTo));
			entityBlackboard->SetValue<UBlackboardKeyType_Vector>(positionID, GetOwner()->GetActorLocation() + Utilities::RandomDisplacementVector(1000));
		}*/
	}
		break;
	case NodeType::destroySelf:
		break;
	case NodeType::flee:
		if(ValidateObject(n->nBlackboard.actor))
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.actor);
		else
			AbortNode();
		break;
	case NodeType::stopFight:
	{
		if (ValidateObject(n->nBlackboard.entity) && ValidateObject(n->nBlackboard.entity->GetOwner())) {
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Entity"), n->nBlackboard.entity);
			entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Actor"), n->nBlackboard.entity->GetOwner());
			UItem* item = n->nBlackboard.item;
			if (item) {
				if (item->IsA<UOEntity>())
					entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("AnotherEntity"), dynamic_cast<UOEntity*>(item));
				else if (item->IsA<UOResidence>())
					entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Residence"), dynamic_cast<UOResidence*>(item));
				else if (item->IsA<UOEdification>())
					entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Edification"), dynamic_cast<UOEdification*>(item));
				//entityBlackboard->SetValue<UBlackboardKeyType_Object>(entityBlackboard->GetKeyID("Item"), item);
			}
			else
				AbortNode();
		}
		else
			AbortNode();
	}
		break;
	default:
		AbortNode();
		break;
	}
}



void AEntityAIController::SetState(UOEntity::AIState s) {

	UOEntity* entity = GetPawn()->FindComponentByClass<UOEntity>();
	if (entity && !entity->IsDead && entityBlackboard) {

		UBlackboardKeyType_Enum::FDataType blackboardState = static_cast<UBlackboardKeyType_Enum::FDataType>(s);
		if (blackboardState) {
			entityBlackboard->SetValue<UBlackboardKeyType_Enum>(entityBlackboard->GetKeyID("EntityState"), blackboardState);
			if (s == UOEntity::AIState::numb)
				entityBlackboard->ClearValue(nodeTypeID);
		}	
	}
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

FVector AEntityAIController::RandomDisplacementVector(int radius) {
	return FVector(rand() % (2 * radius) - radius, rand() % (2 * radius) - radius, 0);
}









UOOwnable* AEntityAIController::GetOwnable(UOEntity* entity, OntologicFunctions::AffordableUse affordableUse, bool isHighPriority) {

	const int STRENGTH_TO_WEIGHT = 2;
	const int APPRECIATION_TO_BORROW = 60;
	const int FEAR_TO_BORROW = 70;
	const int RESPECT_TO_BORROW = 80;


	//AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	//UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	OntologicFunctions ontF;
	
	// Check hands
	UOOwnable* hands = entity->GetHands();
	UOOwnable* bestChoice = hands;
	int bestChoiceAffordance = ontF.GetAffordance(affordableUse, bestChoice, entity);
	bool bestChoiceSomeoneWhoCares = true;

	// Check Grabbed Item
	if (entity->HasGrabbedItem()) {
		UOOwnable* grabbedItem = (UOOwnable*)entity->GetGrabbedItem();
		int grabbedItemAffordance = ontF.GetAffordance(affordableUse, grabbedItem, entity);
		if (grabbedItemAffordance > bestChoiceAffordance) {
			bestChoice = grabbedItem;
			bestChoiceAffordance = grabbedItemAffordance;
		}
	}

	//Initialize candidates
	vector<UOOwnable*> candidates;
	for (OOwnership* o : entity->GetPossessions()) {
		if (!ValidateObject(o->GetOwner()) || !ValidateObject(o->GetOwnable()))
			continue;
		candidates.push_back(o->GetOwnable());
	}
	for (UOOwnable* o : FindNearbyOwnables(entity)) {
		if (!ValidateObject(o))
			continue;
		candidates.push_back(o);
	}

	//Ponder candidates
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
						if (relation->GetAppreciation() >= APPRECIATION_TO_BORROW || relation->GetFear() >= FEAR_TO_BORROW || relation->GetRespect() >= RESPECT_TO_BORROW) {
							borrow = true;
							break;
						}
					}
				}
				if (someoneWhoCares && !borrow) {
					continue;
				}
			}
			int newAffordance = ontF.GetAffordance(affordableUse, ownable, entity);
			if (newAffordance > bestChoiceAffordance) {
				bestChoice = ownable;
				bestChoiceAffordance = newAffordance;
				bestChoiceSomeoneWhoCares = someoneWhoCares;
			}
		}
	}

	if (bestChoice != hands) {
		// Moved to: Entity->GrabItem();
		/*if (!bestChoiceSomeoneWhoCares && entity->GetPersonality()->GetMaterialist() > 50) {
			//create ownership!
			entity->AddPossession(bestChoice);
		}*/

		if (bestChoice != hands && bestChoice != entity->GetGrabbedItem())
		{
			//blackboard->SetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Item"), (UItem*)bestChoice);
			//if (bestChoice->GetOwner())
			//	blackboard->SetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Actor"), bestChoice->GetOwner());

			bestChoice->AddGrabber(entity);
			hands = nullptr;
			return bestChoice;
		}
	}
	
	hands = nullptr;
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
		if (o) {
			bool admit = true;

			for (UOOwnable* own : results) {
				if (o == own) {
					admit = false;
				}
			}

			if (admit && !o->IsA<UOEdification>() && !o->GetIsGrabbed()) {

				vector<UOEntity*> grabbers = o->GetGrabbers();
				for (UOEntity* g : grabbers) {
					ORelation* r = entity->GetRelationWith(g);
					if (r && (r->GetAppreciation() > 25 || r->GetRespect() > 37 || r->GetFear() > 25)) {
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

void AEntityAIController::ReleaseGrabbedItemNearestToHome(UOEntity* entity, FVector targetPosition) {
	if (entity->HasGrabbedItem()) {
		if ((entity->GetGrabbedItem()->GetOwner()->GetActorLocation() - entity->GetHome()->GetOwner()->GetActorLocation()).Size() > (entity->GetOwner()->GetActorLocation() - entity->GetHome()->GetOwner()->GetActorLocation()).Size()) {
			entity->ReleaseGrabbedItem();
		}
	}
}

void AEntityAIController::AbortNode() {
	entityBlackboard->SetValue<UBlackboardKeyType_Enum>(nodeTypeID, static_cast<UBlackboardKeyType_Enum::FDataType>(NodeType::numb));
}

bool AEntityAIController::ValidateObject(UObject* obj) {
	if (obj == nullptr)
		return false;

	if (obj->IsA<UItem>())
		return ((UItem*)obj)->IsValidItem();

	return true;
}