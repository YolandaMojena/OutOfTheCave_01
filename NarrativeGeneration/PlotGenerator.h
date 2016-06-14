// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
#include "BasePlot.h"
#include "PlotTypes.h"
#include <unordered_map>
#include "Report.h"
#include "StringCollection.h"
#include "PlotGenerator.generated.h"

using namespace std;

UCLASS()
class OUTOFTHECAVE_01_API APlotGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	class PlotDictionary {

	public:
		PlotDictionary();
		vector<string> GetPlotsOfType(BasePlot::TypeOfPlot type);

	private:
		unordered_map<BasePlot::TypeOfPlot, vector<string>> _plotDictionary;
		StringCollection strings;
	};

	APlotGenerator();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	void SpawnReactivePlot();

	void AddReportToLog(Report* newReport);
	bool ValidateReport(Report* report);

	PlotDictionary plotDictionary;

	vector<BasePlot*> reactivePlots;
	vector<BasePlot*> ambitionPlots;
	vector<BasePlot*> worldPlots;
	StringCollection strings;

private:

	bool ContainsReport(Report* newReport);
	void GetPlotFromReportLog();
	vector<UOEntity*> WeHaveALotInCommon(Report* report);

	TArray<Report*> _pReportLog;
	bool _lastPlotCompleted = true;
	float _timeToSpawnPlot;

	const float _TIME_TO_SPAWN = 25.0f;
};

