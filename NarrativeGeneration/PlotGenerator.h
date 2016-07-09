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
#include "NarrativeGeneration/Ambition.h"
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

	struct intTest
	{
		bool operator()(int& A, int& B) const
		{
			// Inverted compared to std::priority_queue - higher priorities float to the top
			return A >= B;
		}
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlotGenerator)
		FString currentPlot;
	
	float GetDaytime();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float dayTime = 0;

	APlotGenerator();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	void AddReportToLog(Report* newReport);
	void ChangeCurrentPlotsInAction(int dif);
	vector<UOEntity*> SpawnEntities(int num, ERace race);
	vector<UOOwnable*> GetValuables();
	void AddValuable(UOOwnable* valuable);
	vector<UOEntity*> GetNotoriousEntitiesByRace(ERace race);
	vector<UOEntity*> GetNotoriousEntities();
	void DeleteNotorious(UOEntity* entity);
	void AddNotorious(UOEntity* notorious);

private:

	bool ContainsReport(Report* newReport);
	void GetPlotFromReportLog();
	vector<UOEntity*> WeHaveALotInCommon(Report* report);
	FVector RandomDisplacement(int radius);

	bool SpawnReactivePlot();
	bool SpawnAmbitionPlot();
	bool SpawnWorldPlot();

	PlotDictionary plotDictionary;
	StringCollection strings;

	vector<BasePlot*> _reactivePlots;
	vector<BasePlot*> _ambitionPlots;
	vector<BasePlot*> _worldPlots;

	TArray<Report*> _pReportLog;
	bool _lastPlotCompleted = true;
	float _timeToSpawnPlot;
	int _currentPlotsInAction;

	TSubclassOf<class ACharacter> BP_Bear;
	vector<UOOwnable*> _valuables;
	TArray<UOEntity*> _notoriousEntities;


	const float _TIME_TO_SPAWN = 20.0f;
	const int _MAX_NOTORIOUS = 15;
	const int _MAX_PLOTS = 3;

	bool ValidateAttackPlot(AttackPlot* plot);
	bool ValidateBuildPlot(BuildPlot* build);
	bool ValidateDestroyPlot(DestroyPlot* plot);

	FVector _stampedeTargetLocation;
};



