// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OResidence.h"
#include "Village.h"
#include "Ontology/OEntity.h"

UOResidence::UOResidence() {
	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Ocivilian_Goblin.BP_OCivilian_Goblin'"));
	if (ItemBlueprint.Object) {
		BP_Civilian_Goblin = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
}

UOResidence::~UOResidence() {

}


// Called when the game starts
void UOResidence::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SpawnTenants();
	initialized = true;
}


void UOResidence::SpawnTenants() {
	for (int i = 0; i < numTenants; i++) {
		// Spawn Params -> https://wiki.unrealengine.com/Templates_in_C%2B%2B
		ACharacter* tentantCharacter = GetTentantCharacterFromRace();
		UOEntity* ten = tentantCharacter->FindComponentByClass<UOEntity>();

		// SET RELATIONS
		for (UOEntity* ent : tentants) {
			ORelation* rel = new ORelation(ten, ent);
			ten->AddRelationship(rel);
			rel = new ORelation(ent, ten);
			ent->AddRelationship(rel);
		}

		if (villageID != 0) {
			for (UOEdification* edf : _village->edifications) {
				UOResidence* res = (UOResidence*) edf;
				for (UOEntity* ent : res->tentants) {
					ORelation* rel = new ORelation(ten, ent);
					ten->AddRelationship(rel);
					rel = new ORelation(ent, ten);
					ent->AddRelationship(rel);
				}
			}
		}

		// SET OWNERSHIP WITH THE EDIFICATION
		ten->AddPossession(new OOwnership(ten, ((UOOwnable*)this), 25 + ten->GetPersonality()->GetMaterialist()));

		tentants.push_back(ten);
	}
	_village->edifications.push_back(this);
}

ACharacter* UOResidence::GetTentantCharacterFromRace() {
	FActorSpawnParameters SpawnParams;
	AActor* compOwner = GetOwner();
	ACharacter* tentantCharacter;
	switch (race) {
	case ERace::R_Human:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	case ERace::R_Goblin:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	case ERace::R_Mixt_HG:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	case ERace::R_Bear:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	case ERace::R_Beast:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	case ERace::R_Boar:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	case ERace::R_Cow:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	case ERace::R_Goat:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	default:
		tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 - 100, rand() % 200 - 100, 100), compOwner->GetActorRotation(), SpawnParams);
		break;
	}
	return tentantCharacter;
}

Graph* UOResidence::SetIdleFromJob() {
	Graph* idleGraph = new Graph();
	Node* n = new Node();

	switch (job) {
	case EJob::J_Farmer:
		break;
	case EJob::J_Ironsmith:
		break;
	case EJob::J_Miner:
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
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation());  n->SetDaytime(8);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification(this); n->SetDaytime(8);
		idleGraph->AddNode(n);
		//MAÑANA
		//n = new Node();
		//n->SetNodeType(NodeType::get); n->SetArquetypeObject("azada"); n->SetDaytime(13);
		//idleGraph->AddNode(n);
		if (peasantField) {
			n = new Node();
			n->SetNodeType(NodeType::goTo); n->SetPosition(peasantField->GetOwner()->GetActorLocation());  n->SetDaytime(13);
			idleGraph->AddNode(n);
			n = new Node();
			n->SetNodeType(NodeType::interact); n->SetEdification(peasantField);  n->SetDaytime(13);
			idleGraph->AddNode(n);
		}
		//MEDIODÍA
		n = new Node();
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation());  n->SetDaytime(16);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification((UOEdification*)this);  n->SetDaytime(16);
		idleGraph->AddNode(n);
		//TARDE
		n = new Node();
		n->SetNodeType(NodeType::get); n->SetArquetypeObject("azada"); n->SetDaytime(21);
		idleGraph->AddNode(n);
		if (peasantField) {
			n = new Node();
			n->SetNodeType(NodeType::goTo); n->SetPosition(peasantField->GetOwner()->GetActorLocation());  n->SetDaytime(21);
			idleGraph->AddNode(n);
			n = new Node();
			n->SetNodeType(NodeType::interact); n->SetEdification(peasantField);  n->SetDaytime(21);
			idleGraph->AddNode(n);
		}
		//NOCHE
		n->SetNodeType(NodeType::goTo); n->SetPosition(this->GetOwner()->GetActorLocation());  n->SetDaytime(8);
		idleGraph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::enter); n->SetEdification(this); n->SetDaytime(8);
		idleGraph->AddNode(n);
	}
		break;
	case EJob::J_Shaman:
		break;
	case EJob::J_Soldier:
		break;

	case EJob::J_Herbibore:
		break;
	case EJob::J_Predator:
		break;
	}

	return idleGraph;
}

void UOResidence::IWantToGetInside(UOEntity* e) {
	_inside.push_back(e);
}

void UOResidence::IWantToGetOut(UOEntity* e) {
	int i = 0;
	for (UOEntity* ent : _inside) {
		if (ent == e)
			break;
		i++;
	}
	_inside.erase(_inside.begin() + i);
}