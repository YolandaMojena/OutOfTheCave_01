// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include "NarrativeGeneration/Utilities.h"
#include "Graph.h"
#include "Ontology/Item.h"
#include "Ontology/OOwnable.h"
#include "StringCollection.h"
//#include "NarrativeGeneration/Ambition.h"

using namespace std;

class UOEntity;
enum TypeOfAmbition;

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

	void PrintSentence();
	void AddInvolvedInPlot(UOEntity* entity);
	void SavePlotToFile(const FString path, const FString fileName);
	void AbortPlot(const FString path, const FString fileName);

	virtual void BuildSentence() = 0;
	//virtual bool ValidatePlot() = 0;
	virtual void InitPlot() = 0;

	Graph GetGraph();
	UOEntity* GetMainEntity();
	vector<UOEntity*> GetInvolvedInPlot();
	bool GetIsExclusive();
	//bool GetPlotIsValid();


protected:
	
	FString _sentence;
	FString _identifier;
	UOEntity* _plotEntity;
	Graph _plotGraph;
	vector<UOEntity*> _involvedInPlot;
	UItem* _motivation;
	TypeOfAmbition _ambition;
	bool _isExclusive;
	bool _isPlotValid;

	virtual void BuildGraph() = 0;
	virtual void ConsiderReactions() = 0;
};
