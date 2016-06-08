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

	/*if (_lastPlotCompleted) {

		_timeToSpawnPlot += DeltaTime;

		if (_timeToSpawnPlot >= _TIME_TO_SPAWN) {
			_timeToSpawnPlot = 0;
			_lastPlotCompleted = false;
			SpawnPlot();
		}
	}*/

	SpawnPlot();
}

bool APlotGenerator::ValidatePlot(BasePlot * candidatePlot)
{
	return false;
}

void APlotGenerator::SpawnPlot()
{
	if (reactivePlots.size() > 0) {
		BasePlot* currentPlot = reactivePlots[reactivePlots.size() - 1];
		currentPlot->PrintSentence();
		reactivePlots.pop_back();
	}
	else {
		GetPlotFromReportLog();
		//BasePlot* currentPlot = reactivePlots[reactivePlots.size() - 1];
		//if(currentPlot) reactivePlots.pop_back();
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "PLOT");
	}
}

void APlotGenerator::AddReportToLog(Report* newReport)
{
	if (!ContainsReport(newReport)) {
		_pReportLog.HeapPush(newReport, Report::ReportNotoriety());
	}
}

bool APlotGenerator::ContainsReport(Report* newReport) {

	for (int i = 0; i < _pReportLog.Num(); i++){
		if (newReport->GetTag() == _pReportLog[i]->GetTag()) {
			if (newReport->GetTag() == Report::ReportTag::relation) {
				if (_pReportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _pReportLog[i]->GetTargetEntity() == newReport->GetTargetEntity() && _pReportLog[i]->GetMotivation() == newReport->GetMotivation() && _pReportLog[i]->GetType() == newReport->GetType()) {
					return true;
				}
			}
			else if (newReport->GetTag() == Report::ReportTag::ownership){
				if (_pReportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _pReportLog[i]->GetTargetOwnable() == newReport->GetTargetOwnable() && _pReportLog[i]->GetMotivation() == newReport->GetMotivation() && _pReportLog[i]->GetType() == newReport->GetType()) {
						return true;
				}
			}
		}
	}
	return false;
}


void APlotGenerator::GetPlotFromReportLog() {

	if (_pReportLog.Num() > 0) {

		Report* currentReport;
		_pReportLog.HeapPop(currentReport, Report::ReportNotoriety());

		vector<string> plotCandidates = plotDictionary.GetPlotsOfType(currentReport->GetType());

		// For now, random among plots of the same type
		// Maybe depend on motivation or personality?
		int randType = rand() % plotCandidates.size();
		string plot = plotCandidates[randType];
		BasePlot* newPlot;

		if (plot == strings.ATTACK_PLOT) {
			newPlot = new AttackPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity());
			//newPlot->PrintSentence();
			for (UOEntity* entity : WeHaveALotInCommon(currentReport))
				newPlot->AddInvolvedInPlot(entity);
			reactivePlots.push_back(newPlot);
		}
		else if (plot == strings.DESTROY_PLOT) {
		}
		else if (plot == strings.GATHER_PLOT) {
			newPlot = new GatherPlot(currentReport->GetReportEntity(), currentReport->GetTargetOwnable());
			for (UOEntity* entity : WeHaveALotInCommon(currentReport))
				newPlot->AddInvolvedInPlot(entity);
			reactivePlots.push_back(newPlot);
		}
		else if (plot == strings.ROBBERY_PLOT) {
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

vector<UOEntity*> APlotGenerator::WeHaveALotInCommon(Report* report) {

	Report::ReportTag tag = report->GetTag();
	vector<UOEntity*> helpers;

	int i = 0;

	while (i < _pReportLog.Num()) {

		if (_pReportLog[i]->GetTag() == tag && tag == Report::ReportTag::ownership) {
			if ( _pReportLog[i]->GetType() == report->GetType() && _pReportLog[i]->GetTargetOwnable() == report->GetTargetOwnable()) {
				helpers.push_back(_pReportLog[i]->GetReportEntity());
				_pReportLog.RemoveAt(i);
			}
			else i++;
		}
		else if (_pReportLog[i]->GetTag() == tag && tag == Report::ReportTag::relation) {
			if ( _pReportLog[i]->GetType() == report->GetType() && _pReportLog[i]->GetTargetEntity() == report->GetTargetEntity()) {
				helpers.push_back(_pReportLog[i]->GetReportEntity());
				_pReportLog.RemoveAt(i);
			}
			else i++;
		}
	}

	return helpers;
}
