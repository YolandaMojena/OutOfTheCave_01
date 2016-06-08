// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OEdification.h"
#include "Village.h"
#include "Ontology/OEntity.h"

UOEdification::UOEdification() : UOOwnable() {

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
}


void UOEdification::FindVillage() {
	if (villageID != 0) {
		for (TActorIterator<AVillage> ActorItr(GetOwner()->GetWorld()); ActorItr; ++ActorItr) {
			AVillage* potentialVillage = *ActorItr;
			if (potentialVillage->villageID == villageID)
				_village = potentialVillage;
		}
	}
}









