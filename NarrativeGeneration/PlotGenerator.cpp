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
	if (reactivePlots.size() > 0) {
		BasePlot* currentPlot = reactivePlots[reactivePlots.size() - 1];
		reactivePlots.pop_back();
		currentPlot->ExecutePlot();
	}
}

void APlotGenerator::AddReportToLog(Report* newReport)
{
	if (!ContainsReport(newReport))
		_pReportLog.HeapPush(newReport, Report::ReportNotoriety());
}

bool APlotGenerator::ContainsReport(Report* newReport) {

	for (int i = 0; i < _pReportLog.Num(); i++){
		if (newReport->GetTag() == _pReportLog[i]->GetTag()) {
			if (newReport->GetTag() == Report::ReportTag::relation) {
				if (_pReportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _pReportLog[i]->GetTargetEntity() == newReport->GetTargetEntity() && _pReportLog[i]->GetMotivation() == newReport->GetMotivation() && _pReportLog[i]->GetTypes() == newReport->GetTypes()) {
					return true;
				}
			}
			else if (newReport->GetTag() == Report::ReportTag::ownership){
				if (_pReportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _pReportLog[i]->GetTargetOwnable() == newReport->GetTargetOwnable() && _pReportLog[i]->GetMotivation() == newReport->GetMotivation() && _pReportLog[i]->GetTypes() == newReport->GetTypes()) {
						return true;
				}
			}
		}
	}
	return false;
}


void APlotGenerator::GetPlotFromReport(Report* report) {

	Report* currentReport;
	_pReportLog.HeapPop(currentReport, Report::ReportNotoriety());

	vector<string> plotCandidates;

	for (BasePlot::TypeOfPlot type : report->GetTypes()) 
		plotCandidates.insert(plotCandidates.end(), plotDictionary.GetPlotsOfType(type).begin(), plotDictionary.GetPlotsOfType(type).end());

	// How to choose? 
	for (string plot : plotCandidates) {
		if (plot == strings.ATTACK_PLOT) {
			reactivePlots.push_back(new AttackPlot(report->GetReportEntity(), report->GetTargetEntity()));
		}
		if (plot == strings.DESTROY_PLOT) {
			///
		}
		if (plot == strings.GATHER_PLOT) {
			reactivePlots.push_back(new GatherPlot(report->GetReportEntity(), report->GetTargetOwnable()));
		}
		if (plot == strings.ROBBERY_PLOT) {
			///
		}
	}	
}

APlotGenerator::PlotDictionary::PlotDictionary() {

	_plotDictionary = { {BasePlot::TypeOfPlot::aggressive, {strings.ATTACK_PLOT, strings.DESTROY_PLOT}},
	{ BasePlot::TypeOfPlot::resources, { strings.GATHER_PLOT } },
	{ BasePlot::TypeOfPlot::possessive , { strings.ROBBERY_PLOT } } };
}

vector<string> APlotGenerator::PlotDictionary::GetPlotsOfType(BasePlot::TypeOfPlot type)
{
	return _plotDictionary.at(type);
}
