// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OResidence.h"
#include "Village.h"
#include "Ontology/OEntity.h"

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
}

UOResidence::~UOResidence() {

}


// Called when the game starts
void UOResidence::BeginPlay()
{
	Super::BeginPlay();

	srand(time(NULL));

	// ...
	SpawnTenants();
	initialized = true;
}


void UOResidence::SpawnTenants() {
	for (int i = 0; i < numTenants; i++) {
		// Spawn Params -> https://wiki.unrealengine.com/Templates_in_C%2B%2B
		ACharacter* tentantCharacter = GetTentantCharacterFromRace();

		if (tentantCharacter) {
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
					UOResidence* res = (UOResidence*)edf;
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

			ten->SetIdleGraph(GenerateIdleFromJob());
			ten->SetState(UOEntity::State::idle);

			tentants.push_back(ten);
		}
	}
	_village->edifications.push_back(this);
}

ACharacter* UOResidence::GetTentantCharacterFromRace() {
	FActorSpawnParameters SpawnParams;
	AActor* compOwner = GetOwner();
	ACharacter* tentantCharacter;
	switch (race) {
	case ERace::R_Human:
		if((rand() % 10) < 5)
			tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Human_Male, compOwner->GetActorLocation() + RandomDisplacementVector(100), compOwner->GetActorRotation(), SpawnParams);
		else 
			tentantCharacter = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Human_Female, compOwner->GetActorLocation() + RandomDisplacementVector(100), compOwner->GetActorRotation(), SpawnParams);
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

Graph* UOResidence::GenerateIdleFromJob() {
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
			n->SetNodeType(NodeType::goTo); n->SetPosition(peasantField->GetOwner()->GetActorLocation() + RandomDisplacementVector(400));  n->SetDaytime(13);
			idleGraph->AddNode(n);
			n = new Node();
			n->SetNodeType(NodeType::cultivate); /*n->SetEdification(peasantField);*/  n->SetDaytime(13);
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
			n->SetNodeType(NodeType::goTo); n->SetPosition(peasantField->GetOwner()->GetActorLocation() + RandomDisplacementVector(400));  n->SetDaytime(21);
			idleGraph->AddNode(n);
			n = new Node();
			n->SetNodeType(NodeType::cultivate); /*n->SetEdification(peasantField);*/  n->SetDaytime(21);
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

FVector UOResidence::RandomDisplacementVector(int radius){
	return FVector(rand() % (2 * radius) - radius, rand() % (2 * radius) - radius, 0);
}