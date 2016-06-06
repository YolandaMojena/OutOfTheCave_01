// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OEdification.h"
#include "Village.h"
#include "Ontology/OEntity.h"

UOEdification::UOEdification() : UOOwnable() {

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Ocivilian_Goblin.BP_OCivilian_Goblin'"));
	if (ItemBlueprint.Object) {
		BP_Civilian_Goblin = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}

}

UOEdification::~UOEdification()
{
}

// Called when the game starts
void UOEdification::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FindVillage();
	SpawnTenants();
	
	initialized = true;
}

void UOEdification::SpawnTenants() {
	if (race == ERace::R_Goblin) {
		for (int i = 0; i < numTenants; i++) {
			// Spawn Params -> https://wiki.unrealengine.com/Templates_in_C%2B%2B
			FActorSpawnParameters SpawnParams;
			AActor* compOwner = GetOwner();
			ACharacter* civilian = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation() + FVector(rand() % 200 -100, rand() % 200-100, 100), compOwner->GetActorRotation(), SpawnParams);
			UOCivilian* civilianComp = civilian->FindComponentByClass<UOCivilian>();
			UOEntity* ten = (UOEntity*)civilianComp;

			for (UOEntity* ent : tentants) {
				ORelation* rel = new ORelation(ten, ent);
				ten->AddRelationship(rel);
				rel = new ORelation(ent, ten);
				ent->AddRelationship(rel);
			}

			for (UOEdification* edf : _village->edifications) {
				for (UOEntity* ent : edf->tentants) {
					ORelation* rel = new ORelation(ten, ent);
					ten->AddRelationship(rel);
					rel = new ORelation(ent, ten);
					ent->AddRelationship(rel);
				}
			}

			ten->AddPossession(new OOwnership(ten, ((UOOwnable*)this), 25 + ten->GetPersonality()->GetMaterialist()));

			tentants.push_back(ten);
		}
	}
	_village->edifications.push_back(this);
}

void UOEdification::FindVillage() {
	for (TActorIterator<AVillage> ActorItr(GetOwner()->GetWorld()); ActorItr; ++ActorItr) {
		AVillage* potentialVillage = *ActorItr;
		if (potentialVillage->villageID == villageID)
			_village = potentialVillage;
	}
}






