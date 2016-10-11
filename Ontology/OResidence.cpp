// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OResidence.h"
#include "Village.h"
#include "Ontology/OEntity.h"
#include "Ontology/OCivilian.h"

UOResidence::UOResidence() {

	// Goblin
	static ConstructorHelpers::FObjectFinder<UBlueprint> GoblinBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Ocivilian_Goblin.BP_OCivilian_Goblin'"));
	if (GoblinBlueprint.Object) {
		BP_Civilian_Goblin = (UClass*)GoblinBlueprint.Object->GeneratedClass;
	}

	// Male
	static ConstructorHelpers::FObjectFinder<UBlueprint> MaleBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_OCivilian_Human_Male.BP_OCivilian_Human_Male'"));
	if (MaleBlueprint.Object) {
		BP_Civilian_Human_Male = (UClass*)MaleBlueprint.Object->GeneratedClass;
	}

	// Female
	static ConstructorHelpers::FObjectFinder<UBlueprint> FemaleBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_OCivilian_Human_Female.BP_OCivilian_Human_Female'"));
	if (FemaleBlueprint.Object) {
		BP_Civilian_Human_Female = (UClass*)FemaleBlueprint.Object->GeneratedClass;
	}

	// Herbivore
	static ConstructorHelpers::FObjectFinder<UBlueprint> HerbivoreBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Herbivore.BP_Herbivore'"));
	if (HerbivoreBlueprint.Object) {
		BP_Herbivore = (UClass*)HerbivoreBlueprint.Object->GeneratedClass;
	}

	// Bear
	static ConstructorHelpers::FObjectFinder<UBlueprint> BearBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Bear.BP_Bear'"));
	if (BearBlueprint.Object) {
		BP_Bear = (UClass*)BearBlueprint.Object->GeneratedClass;
	}

	// Wolf
	static ConstructorHelpers::FObjectFinder<UBlueprint> WolfBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Wolf.BP_Wolf'"));
	if (WolfBlueprint.Object) {
		BP_Wolf = (UClass*)WolfBlueprint.Object->GeneratedClass;
	}
}

UOResidence::~UOResidence() {

}


// Called when the game starts
void UOResidence::BeginPlay()
{
	Super::BeginPlay();

	srand(time(NULL));

	//SpawnTenants();
	//initialized = true;
	
}

void UOResidence::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!_plotGenerator) {
		for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
			_plotGenerator = *Itr;
		SpawnTenants();
		initialized = true;
	}
}


void UOResidence::SpawnTenants() {
	for (int i = 0; i < numTenants; i++) {
		// Spawn Params -> https://wiki.unrealengine.com/Templates_in_C%2B%2B
		ACharacter* tentantCharacter = GetTentantCharacterFromRace();
		if (tentantCharacter) {
			UOEntity* ten = tentantCharacter->FindComponentByClass<UOEntity>();

			ten->SetJob(job);
			ten->SetRace(race);
			ten->GenerateTraits();
			ten->SetHome(this);

			if (ten) {
				// SET RELATIONS
				bool chief = true;
				for (UOEntity* ent : tentants) {
					/*ORelation* rel = new ORelation(ten, ent);
					ten->AddRelationship(rel);
					rel = new ORelation(ent, ten);
					ent->AddRelationship(rel);*/
					ORelation* rel = ten->AddRelationship(ent);
					if (chief) {
						rel->ChangeRespect(50 + rand() % 50);
						chief = false;
					}
					ent->AddRelationship(ten);
				}

				if (villageID != 0) {
					for (UOEdification* edf : _village->edifications) {
						UOResidence* res = (UOResidence*)edf;
						for (UOEntity* ent : res->tentants) {
							/*ORelation* rel = new ORelation(ten, ent);
							ten->AddRelationship(rel);
							rel = new ORelation(ent, ten);
							ent->AddRelationship(rel);*/
							ten->AddRelationship(ent);
							ent->AddRelationship(ten);
						}
					}
				}

				// SET OWNERSHIP WITH THE EDIFICATION
				ten->SetIdleGraph(GenerateIdleFromJob());

				ten->SetPlotGenerator();
				ten->RethinkState();
				
				tentants.push_back(ten);
			}
		}
	}
	if(villageID != 0)
		_village->edifications.push_back(this);
}

ACharacter* UOResidence::GetTentantCharacterFromRace() {
	FActorSpawnParameters SpawnParams;
	AActor* compOwner = GetOwner();
	ACharacter* tentantCharacter;
	switch (race) {
	case ERace::R_Human: {
		if ((rand() % 10) < 5) {
			tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Human_Male, compOwner->GetActorLocation() + RandomDisplacementVector(100), compOwner->GetActorRotation(), SpawnParams);
			UOEntity* entityComp = tentantCharacter->FindComponentByClass<UOEntity>();
			if (entityComp && villageID != 0 && _village) {
				entityComp->SetItemName(_village->AssignMaleHumanName());
				entityComp->SetRace(race);
			}	
		}
		else {
			tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Human_Female, compOwner->GetActorLocation() + RandomDisplacementVector(100), compOwner->GetActorRotation(), SpawnParams);
			UOEntity* entityComp = tentantCharacter->FindComponentByClass<UOEntity>();
			if (entityComp && villageID != 0 && _village) {
				entityComp->SetItemName(_village->AssignFemaleHumanName());
				entityComp->SetRace(race);
			}		
		}
	}
	break;
	case ERace::R_Goblin:
	{
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + RandomDisplacementVector(100), compOwner->GetActorRotation(), SpawnParams);
		UOEntity* entityComp = tentantCharacter->FindComponentByClass<UOEntity>();
		if (entityComp && villageID != 0 &&_village) {
			entityComp->SetItemName(_village->AssignGoblinName());
			entityComp->SetRace(race);
		}
	}
		break;
	case ERace::R_Bear:
	{
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Bear, compOwner->GetActorLocation() + RandomDisplacementVector(500), compOwner->GetActorRotation(), SpawnParams);
		UOEntity* entityComp = tentantCharacter->FindComponentByClass<UOEntity>();
		if (entityComp->IsValidItem())
			entityComp->SetItemName("Bear");
	}
		break;
	case ERace::R_Wolf:
	{
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Wolf, compOwner->GetActorLocation() + RandomDisplacementVector(500), compOwner->GetActorRotation(), SpawnParams);
		UOEntity* entityComp = tentantCharacter->FindComponentByClass<UOEntity>();
		if (entityComp->IsValidItem())
			entityComp->SetItemName("Wolf");
	}
		break;
	case ERace::R_Herbivore:
	{
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Herbivore, compOwner->GetActorLocation() + RandomDisplacementVector(500), compOwner->GetActorRotation(), SpawnParams);
		UOEntity* entityComp = tentantCharacter->FindComponentByClass<UOEntity>();
		if (entityComp->IsValidItem())
			entityComp->SetItemName("Worm");
	}
		break;
	default:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + RandomDisplacementVector(100), compOwner->GetActorRotation(), SpawnParams);
		break;
	}

	tentantCharacter->SetActorLocation(tentantCharacter->GetActorLocation() * FVector(1, 1, 0) + FVector(0, 0, 100));

	return tentantCharacter;
}

Graph* UOResidence::GenerateIdleFromJob() {
	Graph* idleGraph = new Graph();
	Node* n = new Node();

	switch (job) {
	case EJob::J_Farmer:
		break;
	case EJob::J_Ironsmith:
		break;
	case EJob::J_Miner:
	{
		UOEdification* minerOre = nullptr;
		for (TObjectIterator<UOEdification> ObjItr; ObjItr; ++ObjItr) {
			UOEdification* aux = *ObjItr;
			if (aux->edificationType == EdificationType::ore && aux->villageID == this->villageID && aux->edificationID == this->edificationID && aux != (UOEdification*)this)
				minerOre = *ObjItr;
		}
		//MADRUGADA
		n->SetNodeType(NodeType::enter); n->SetEdification(this); n->SetDaytime(8);
		idleGraph->AddNode(n);
		//MAÑANA
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::mine); n->SetDaytime(13);
		idleGraph->AddNode(n);
		if (minerOre) {
			n = new Node();
			n->SetNodeType(NodeType::mine); n->SetActor(minerOre->GetOwner()); n->SetDaytime(13);
			idleGraph->AddNode(n);
		}
		//MEDIODÍA
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification((UOEdification*)this);  n->SetDaytime(16);
		idleGraph->AddNode(n);
		//TARDE
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::mine); n->SetDaytime(21);
		idleGraph->AddNode(n);
		if (minerOre) {
			n = new Node();
			n->SetNodeType(NodeType::mine); n->SetActor(minerOre->GetOwner());  n->SetDaytime(21);
			idleGraph->AddNode(n);
		}
		//NOCHE
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification(this); n->SetDaytime(24);
		idleGraph->AddNode(n);
	}
		break;
	case EJob::J_Peasant:
	{
		UOEdification* peasantField = nullptr;
		for (TObjectIterator<UOEdification> ObjItr; ObjItr; ++ObjItr) {
			UOEdification* aux = *ObjItr;
			if (aux->edificationType == EdificationType::field && aux->villageID == this->villageID && aux->edificationID == this->edificationID && aux != (UOEdification*)this)
				peasantField = *ObjItr;
		}

		//MADRUGADA
		n->SetNodeType(NodeType::enter); n->SetEdification(this); n->SetDaytime(8);
		idleGraph->AddNode(n);
		//MAÑANA
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::cultivator); n->SetDaytime(13);
		idleGraph->AddNode(n);
		if (peasantField) {
			n = new Node();
			n->SetNodeType(NodeType::cultivate); n->SetEdification(peasantField);  n->SetDaytime(13);
			idleGraph->AddNode(n);
		}
		//MEDIODÍA
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification((UOEdification*)this);  n->SetDaytime(16);
		idleGraph->AddNode(n);
		//TARDE
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::cultivator); n->SetDaytime(21);
		idleGraph->AddNode(n);
		if (peasantField) {
			n = new Node();
			n->SetNodeType(NodeType::cultivate); n->SetEdification(peasantField);  n->SetDaytime(21);
			idleGraph->AddNode(n);
		}
		//NOCHE
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification(this); n->SetDaytime(24);
		idleGraph->AddNode(n);
	}
		break;
	case EJob::J_Shaman:
		break;
	case EJob::J_Soldier:
	{

		//JORNADA NOCTURNA
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetDaytime(3);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(3);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(3);
		idleGraph->AddNode(n);
		
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetDaytime(6);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(6);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(6);
		idleGraph->AddNode(n);
		
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetDaytime(9);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(9);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(9);
		idleGraph->AddNode(n);
		
		//DESCANSO 1
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification(this); n->SetDaytime(13);
		idleGraph->AddNode(n);

		//JORNADA 2
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetDaytime(15);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(15);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(15);
		idleGraph->AddNode(n);
		
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetDaytime(17);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(17);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(17);
		idleGraph->AddNode(n);

		//DESCANSO 2
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification(this); n->SetDaytime(21);
		idleGraph->AddNode(n);

		//JORNADA LABORAL 2
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetDaytime(24);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(24);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(24);
		idleGraph->AddNode(n);
	}
		break;

	case EJob::J_Herbibore:
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation());  n->SetDaytime(8);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(8);
		idleGraph->AddNode(n);

		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(13);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(13);
		idleGraph->AddNode(n);

		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(18);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(18);
		idleGraph->AddNode(n);

		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(21);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(21);
		idleGraph->AddNode(n);

		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation());  n->SetDaytime(24);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(24);
		idleGraph->AddNode(n);

		break;
	case EJob::J_Predator:
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation());  n->SetDaytime(8);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(8);
		idleGraph->AddNode(n);

		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(12);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::preyUpon); n->SetDaytime(12);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(13);
		idleGraph->AddNode(n);

		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation() + RandomDisplacementVector(1000));  n->SetDaytime(18);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::preyUpon); n->SetDaytime(18);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(19);
		idleGraph->AddNode(n);

		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation());  n->SetDaytime(24);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::waitUntilDaytime); n->SetDaytime(24);
		idleGraph->AddNode(n);

		break;
	}

	return idleGraph;
}

