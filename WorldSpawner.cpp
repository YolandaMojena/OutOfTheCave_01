// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "WorldSpawner.h"
#include "Ontology/OCivilian.h"
#include "Ontology/OPersonality.h"


// Sets default values
AWorldSpawner::AWorldSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Ocivilian.BP_OCivilian_Goblin'"));
	if (ItemBlueprint.Object) {
		BP_Civilian_Goblin = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AWorldSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnCivilians();
}

// Called every frame
void AWorldSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWorldSpawner::SpawnCivilians() {

	// Spawn Params -> https://wiki.unrealengine.com/Templates_in_C%2B%2B
	FActorSpawnParameters SpawnParams;
	ACharacter* civilian = GetWorld()->SpawnActor<ACharacter>(BP_Civilian_Goblin, GetActorLocation(), GetActorRotation(), SpawnParams);
	_civilians.push_back(civilian);

	UOCivilian* civilianComp = civilian->FindComponentByClass<UOCivilian>();

}

