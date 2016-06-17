// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include "NarrativeGeneration/Utilities.h"
#include "Graph.h"
#include "Ontology/OEntity.h"
#include "Ontology/Item.h"
#include "Ontology/OOwnable.h"
#include "StringCollection.h"

using namespace std;

class UOEntity;

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

	void ExecutePlot();
	void PrintSentence();
	void AddInvolvedInPlot(UOEntity* entity);
	void SavePlotToFile(const FString path, const FString fileName);
	virtual void BuildSentence() = 0;

	Graph GetGraph();
	UOEntity* GetMainEntity();
	vector<UOEntity*> GetInvolvedInPlot();
	bool GetIsExclusive();


protected:
	
	FString _sentence;
	UOEntity* _plotEntity;
	Graph _plotGraph;
	vector<UOEntity*> _involvedInPlot;
	UItem* _motivation;
	bool _isExclusive;

	virtual void BuildGraph() = 0;
	virtual void ConsiderReactions() = 0;
};
