// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include "NarrativeGeneration/Utilities.h"
#include "Graph.h"
#include "Ontology/Item.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/Ambition.h"

using namespace std;

class UOEntity;
class UItem;
class APlotGenerator;

enum class TypeOfPlot : uint8 {
	aggressive,
	thankful,
	preventive,
	possessive,
	resources,
	world
};

class OUTOFTHECAVE_01_API BasePlot
{
public:

	BasePlot(); // Constructor used for wold plots that don't involve a specific entity
	BasePlot(UOEntity* plotEntity);
	~BasePlot();

	void PrintSentence(APlotGenerator* plotGenerator, FString motivationName, TypeOfAmbition ambition);
	void AddInvolvedInPlot(UOEntity* entity);
	void SavePlotToFile(const FString path, const FString fileName);
	void AbortPlot(const FString path, const FString fileName);
	void ChangeMainEntity(UOEntity* entity);

	virtual void BuildSentence() = 0;
	virtual void InitPlot() = 0;
	virtual BasePlot* ConsiderReactions() = 0;

	Graph GetGraph();
	Graph* GetGraphPointer();
	UOEntity* GetMainEntity();
	TArray<UOEntity*> GetInvolvedInPlot();
	void DeleteFromInvolved(UOEntity* entity);
	TypeOfAmbition GetPlotTypeOfAmbition();
	bool GetIsExclusive();
	FString GetSentence();
	FString GetMotivationName();
	FString GetMotivationRace();
	TypeOfAmbition GetAmbition();


protected:
	
	FString _sentence;
	FString _identifier;
	UOEntity* _plotEntity;
	Graph _plotGraph;
	TArray<UOEntity*> _involvedInPlot;
	TypeOfAmbition _ambition = TypeOfAmbition::noAmbition;
	bool _isExclusive = false;
	bool _isPlotValid;
	FString _motivationRace;
	FString _motivationName;

	virtual void BuildGraph() = 0;
};
