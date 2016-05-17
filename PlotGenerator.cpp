// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "PlotGenerator.h"


// Sets default values
APlotGenerator::APlotGenerator(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlotGenerator::BeginPlay()
{
	Super::BeginPlay();

	//There's only one instance of the GameWorldManager in the world
	for (TActorIterator<AGameWorldManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		worldManager = *ActorItr;
}

void APlotGenerator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (first) {

		currentPlot = GeneratePlot();
		currentPlot->currentNode = currentPlot->plotGraph.graph["Begin"];
		first = false;
	}

	if (!currentPlot->plotCompleted) currentPlot->ExecutePlot(DeltaTime);

}

// Returns candidate plots by evaluating the preconditions from plots in the collection
// Evaluate Reactive Plots
// Evaluate NPC ambitions
vector<BasePlot*> APlotGenerator::CheckPreconditions()
{
	vector<BasePlot*> candidatePlots;

	// want thing candidates -> Gather

	_wantThing = CheckSimpleRelation(Relation::KindOfRelation::wants);

	if (_wantThing.size() > 0) candidatePlots.push_back(new GatherPlot());

	// hate entity candidates -> Attack

	_hateEntity = CheckSimpleRelation(Relation::KindOfRelation::foe);

	if (_hateEntity.size() > 0) candidatePlots.push_back(new AttackPlot());

	return candidatePlots;
}

BasePlot* APlotGenerator::GeneratePlot() {

	vector<BasePlot*> availablePlots = CheckPreconditions();

	int plotIndex = rand() % availablePlots.size();
	BasePlot* candidatePlot = availablePlots.at(plotIndex);

	if (candidatePlot->name == "GatherPlot")
		candidatePlot->GatherTargets(_wantThing);

	else if (candidatePlot->name == "AttackPlot")
		candidatePlot->GatherTargets(_hateEntity);

	return candidatePlot;
}

vector<AEntity*> APlotGenerator::CheckSimpleRelation(Relation::KindOfRelation kind) {

	vector<AEntity*> candidates;

	for (int i = 0; i < worldManager->worldEntities.size(); i++) {

		for (int j = 0; j < worldManager->worldEntities[i]->relations.size(); j++) {

			if (worldManager->worldEntities[i]->relations[j].relation == kind) {

				candidates.push_back(worldManager->worldEntities[i]);
				break;
			}
		}	
	}

	return candidates;
}

