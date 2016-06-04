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

	if (_lastPlotCompleted) {

		_timeToSpawnPlot += DeltaTime;

		if (_timeToSpawnPlot >= _TIME_TO_SPAWN) {
			_timeToSpawnPlot = 0;
			_lastPlotCompleted = false;
			SpawnPlot();
		}
	}
}

bool APlotGenerator::ValidatePlot(BasePlot * candidatePlot)
{
	return false;
}

void APlotGenerator::SpawnPlot()
{
	
}

void APlotGenerator::AddReportToLog(Report* newReport)
{
	if (!CheckContainsReport(newReport))
		_reportLog.push_back(newReport);
}

bool APlotGenerator::CheckContainsReport(Report* newReport) {

	for (int i = 0; i < _reportLog.size(); i++){

		if (newReport->GetTag() == _reportLog[i]->GetTag()) {

			if (newReport->GetTag() == Report::ReportTag::relation) {

				if (_reportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _reportLog[i]->GetTargetEntity() == newReport->GetTargetEntity() && _reportLog[i]->GetMotivation() == newReport->GetMotivation()) {
					UpdateReport(_reportLog[i], newReport);
					return true;
				}
			}
			else if (newReport->GetTag() == Report::ReportTag::ownership){

				if (_reportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _reportLog[i]->GetTargetOwnable() == newReport->GetTargetOwnable() && _reportLog[i]->GetMotivation() == newReport->GetMotivation()) {
					UpdateReport(_reportLog[i], newReport);
						return true;
				}
			}
		}
	}

	return false;
}

void APlotGenerator::UpdateReport(Report* oldReport, Report* newReport) {

	oldReport = newReport;
}

void APlotGenerator::GetPlotFromReport(Report* report) {

	
}

APlotGenerator::PlotDictionary::PlotDictionary() {

	_plotDictionary = { {BasePlot::TypeOfPlot::aggressive, {_strings.ATTACK_PLOT}},
	{ BasePlot::TypeOfPlot::resources, { _strings.GATHER_PLOT } },
	{ BasePlot::TypeOfPlot::possesive, { _strings.GATHER_PLOT } } };
}

vector<string> APlotGenerator::PlotDictionary::GetPlotsOfType(BasePlot::TypeOfPlot type) {

	return _plotDictionary.at(type);
}



