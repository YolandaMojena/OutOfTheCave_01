// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include "Graph.h"
#include "StringCollection.h"

using namespace std;

class UOEntity;

class OUTOFTHECAVE_01_API BasePlot
{
public:

	enum TypeOfPlot {
		aggressive, thankful, preventive, possessive, resources, world
	};

	BasePlot(); // Constructor used for wold plots that don't involve a specific entity
	BasePlot(UOEntity* plotEntity);
	~BasePlot();

	void ExecutePlot();
	void PrintSentence();
	void AddInvolvedInPlot(UOEntity* entity);

	TypeOfPlot plotTypes;
	BasePlot* additiveReactions;
	BasePlot* negativeReactions;
	StringCollection strings;
	Graph* GetGraph();

protected:
	
	string _description, _sentence;
	UOEntity* _plotEntity;
	Graph* _plotGraph;
	vector<UOEntity*> _involvedInPlot;
	bool _plotCompleted = false;

	virtual void BuildGraph() = 0;
	virtual string BuildSentence() = 0;
	virtual void GatherTargets() = 0;
	virtual void ConsiderReactions() = 0;
};
