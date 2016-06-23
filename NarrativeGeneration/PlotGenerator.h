// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
#include "BasePlot.h"
#include "PlotTypes.h"
#include "Utilities.h"
#include <unordered_map>
#include "Report.h"
#include "StringCollection.h"
#include "PlotGenerator.generated.h"

using namespace std;

class PlotDictionary {

public:
	PlotDictionary();
	vector<string> GetPlotsOfType(TypeOfPlot type);

private:
	unordered_map<TypeOfPlot, vector<string>> _plotDictionary;
	StringCollection strings;
};

UCLASS()
class OUTOFTHECAVE_01_API APlotGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	APlotGenerator();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	void AddReportToLog(Report* newReport);
	bool ValidateReport(Report* report);
	void ChangeCurrentPlotsInAction(int dif);
	vector<UOEntity*> SpawnEntities(int num, ERace race);
	vector<UOOwnable*> GetValuables();
	vector<UOEntity*> GetNotoriousEntitiesByRace(ERace race);

private:

	bool ContainsReport(Report* newReport);
	void GetPlotFromReportLog();
	vector<UOEntity*> WeHaveALotInCommon(Report* report);
	FVector RandomDisplacementVector(int radius);

	void SpawnReactivePlot();
	void SpawnWorldPlot();

	PlotDictionary plotDictionary;
	StringCollection strings;

	vector<BasePlot*> reactivePlots;
	vector<BasePlot*> ambitionPlots;
	vector<BasePlot*> worldPlots;

	TArray<Report*> _pReportLog;
	bool _lastPlotCompleted = true;
	float _timeToSpawnPlot;
	int _currentPlotsInAction;

	TSubclassOf<class ACharacter> BP_Bear;
	vector<UOOwnable*> _valuables;
	TArray<UOEntity*> _notoriousEntities;


	const float _TIME_TO_SPAWN = 5.0f;
	const int _MAX_PLOTS = 3;

	bool ValidateAttackPlot(AttackPlot* plot);
	//bool ValidateBuildPlot(BuildPlot* build);
	bool ValidateDestroyPlot(DestroyPlot* plot);
};



