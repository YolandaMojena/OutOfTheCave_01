// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BasePlot.h"
#include "EngineUtils.h"
#include <vector>
#include "Node.h"
#include "GameWorldManager.h"
#include <PlotTypes.h>
#include "Relation.h"
#include "PlotGenerator.generated.h"

using namespace std;

UCLASS()
class OUTOFTHECAVE_01_API APlotGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	APlotGenerator(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

private:

	vector<BasePlot*> CheckPreconditions();
	BasePlot* GeneratePlot();
	vector<AEntity*> GatherTargets();


	vector<AEntity*> CheckSimpleRelation(Relation::KindOfRelation kindOfRelation);

	BasePlot* currentPlot;
	AGameWorldManager* worldManager;



	bool first = true;

	vector<AEntity*> _wantThing;
	vector<AEntity*> _hateEntity;
};
