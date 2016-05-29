// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "PlotGenerator.h"


// Sets default values
APlotGenerator::APlotGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlotGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlotGenerator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AddReactivePot() {

}


bool APlotGenerator::ValidatePlot(BasePlot * candidatePlot)
{
	return false;
}

void APlotGenerator::SpawnPlot()
{
}

APlotGenerator::PlotDictionary::PlotDictionary() {

	_plotDictionary = { {BasePlot::TypeOfPlot::aggressive, {"attack"}},
	{ BasePlot::TypeOfPlot::resources, {"gather"} },
	{ BasePlot::TypeOfPlot::possesive, {"gather"} } };
}

vector<string> APlotGenerator::PlotDictionary::GetPlotsOfType(BasePlot::TypeOfPlot type) {

	return _plotDictionary.at(type);
}



