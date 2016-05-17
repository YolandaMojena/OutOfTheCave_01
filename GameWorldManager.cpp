// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "GameWorldManager.h"

AGameWorldManager::AGameWorldManager(const FObjectInitializer& ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameWorldManager::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AEntity> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		worldEntities.push_back(*ActorItr);
	}

	for (TActorIterator<AThing> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		worldThings.push_back(*ActorItr);
	}

	InitRelations();
}

// Called every frame
void AGameWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameWorldManager::InitRelations() {

	int prev;
	int next;

	for (int i = 0; i < worldEntities.size(); i++) {

		if (i == 0) {
			prev = worldEntities.size() - 1;
			next = i + 1;
		}
		else if (i == worldEntities.size() - 1) {
			prev = i - 1;
			next = 0;
		}
		else {
			prev = i - 1;
			next = i + 1;
		}

		worldEntities[i]->relations.push_back(Relation(worldEntities[prev], Relation::KindOfRelation::colleague));
		worldEntities[i]->relations.push_back(Relation(worldEntities[next], Relation::KindOfRelation::foe));
		worldEntities[i]->relations.push_back(Relation(worldThings[rand() % worldThings.size()], Relation::KindOfRelation::owns));
		worldEntities[i]->relations.push_back(Relation(worldThings[rand() % worldThings.size()], Relation::KindOfRelation::wants));

		worldRelations.insert(worldRelations.end(), worldEntities[i]->relations.begin(), worldEntities[i]->relations.end());
	}
}



