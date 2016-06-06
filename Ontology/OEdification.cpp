// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OEdification.h"
#include "Ontology/OEntity.h"

UOEdification::UOEdification() : UOOwnable() {

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Ocivilian_Goblin.BP_OCivilian_Goblin'"));
	if (ItemBlueprint.Object) {
		BP_Civilian_Goblin = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}

}

void UOEdification::SpawnTenants() {
	if (raceID == 1) {
		for (int i = 0; i < numTenants; i++) {
			if (rand() % 1 > 0.5f) {
				// Spawn MACHO
				// Spawn Params -> https://wiki.unrealengine.com/Templates_in_C%2B%2B
				FActorSpawnParameters SpawnParams;
				AActor* compOwner = GetOwner();
				ACharacter* civilian = compOwner->GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, compOwner->GetActorLocation(), compOwner->GetActorRotation(), SpawnParams);
				_civilians.push_back(civilian);

				UOCivilian* civilianComp = civilian->FindComponentByClass<UOCivilian>();
				
				_tentants.push_back((UOEntity*)civilianComp);
			}
			else {
				//Spawn HEMBRA
			}
		}
	}
}






