// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
#include <BasePlot.h>
#include <PlotTypes.h>
#include <unordered_map>
#include "Report.h"
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
		vector<string> GetPlotsOfType(BasePlot::TypeOfPlot);

	private:
		unordered_map<BasePlot::TypeOfPlot, vector<string>> _plotDictionary;
	};

	/* Unreal Priority Queue -> https://answers.unrealengine.com/questions/180188/analogue-of-priority-queue.html?sort=oldest */

	APlotGenerator();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	bool ValidatePlot(BasePlot* candidatePlot);
	void SpawnPlot();

	void AddReportToLog(Report* newReport);

	PlotDictionary plotDictionary;

	vector<BasePlot*> reactivePlots;
	vector<BasePlot*> ambitionPlots;
	vector<BasePlot*> worldPlots;

private:

	bool CheckContainsReport(Report* newReport);
	void UpdateReport(Report* oldReport, Report* newReport);

	vector<Report*> _reportLog;
};

