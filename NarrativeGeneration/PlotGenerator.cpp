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

bool APlotGenerator::ValidateReport(Report* report)
{
	/*if (report->GetTag() == Report::ReportTag::relation) {
		return (!report->GetReportEntity()->GetIsDead() || !report->GetTargetEntity()->GetIsDead());
	}
	else if (report->GetTag() == Report::ReportTag::ownership) {
		return (!report->GetReportEntity()->GetIsDead());
	}

	else */return true;
}

void APlotGenerator::SpawnPlot()
{
	if (reactivePlots.size() > 0) {
		BasePlot* currentPlot = reactivePlots.at(0);
		reactivePlots.erase(reactivePlots.begin());
		currentPlot->PrintSentence();
	}
	else GetPlotFromReportLog();
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

// Removes X reports from _pReportLog and converts them into plots
// It is verified that each report is valid before converting it
// When creating the plot, involved entities are obtained from redundant plots and these are deleted from the log
void APlotGenerator::GetPlotFromReportLog() {

	if (_pReportLog.Num() > 0) {

		Report* currentReport;
		_pReportLog.HeapPop(currentReport, Report::ReportNotoriety());

		while (!ValidateReport(currentReport) && _pReportLog.Num() > 0) {
			_pReportLog.HeapPop(currentReport, Report::ReportNotoriety());
		}

		if (ValidateReport(currentReport)) {

			vector<string> plotCandidates = plotDictionary.GetPlotsOfType(currentReport->GetType());

			// A random plot from the given type is elected
			int randType = rand() % plotCandidates.size();
			string plot = plotCandidates[randType];
			BasePlot* newPlot;

			if (plot == strings.ATTACK_PLOT) {
				newPlot = new AttackPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity());
				newPlot->plotEntity = currentReport->GetReportEntity();
				for (UOEntity* entity : WeHaveALotInCommon(currentReport))
					newPlot->AddInvolvedInPlot(entity);
				reactivePlots.push_back(newPlot);
			}
		}
	}
}

// Associates types of plots with plot identificators
APlotGenerator::PlotDictionary::PlotDictionary() {

	_plotDictionary = { {BasePlot::TypeOfPlot::aggressive, {strings.ATTACK_PLOT, strings.DESTROY_PLOT}},
	{ BasePlot::TypeOfPlot::resources, { strings.GATHER_PLOT } },
	{ BasePlot::TypeOfPlot::possessive , { strings.ROBBERY_PLOT } } };
}

vector<string> APlotGenerator::PlotDictionary::GetPlotsOfType(BasePlot::TypeOfPlot type)
{
	return _plotDictionary.at(type);
}

// Looks for coincidences between the given plot and the log
// Entities are returned as helpers
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
