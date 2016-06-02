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

APlotGenerator::PlotDictionary::PlotDictionary() {

	_plotDictionary = { {BasePlot::TypeOfPlot::aggressive, {"attack"}},
	{ BasePlot::TypeOfPlot::resources, {"gather"} },
	{ BasePlot::TypeOfPlot::possesive, {"gather"} } };
}

vector<string> APlotGenerator::PlotDictionary::GetPlotsOfType(BasePlot::TypeOfPlot type) {

	return _plotDictionary.at(type);
}



