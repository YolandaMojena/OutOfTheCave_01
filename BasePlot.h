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

	BasePlot(); // Constructor used for wold plots that don't involve a specific entity
	BasePlot(UOEntity* plotEntity);
	~BasePlot();

	enum TypeOfPlot {
		aggressive, thankful, preventive, possessive, resources, world
	};

	virtual void GatherTargets() = 0;
	virtual void ConsiderReactions() = 0;

	
	void ExecutePlot();
	
	void PrintSentence();

	TypeOfPlot plotTypes;
	BasePlot* additiveReactions;
	BasePlot* negativeReactions;
	UOEntity* plotEntity;
	vector<UOEntity*> involvedInPlot;
	StringCollection strings;

protected:
	
	string _name, _description, _sentence;
	Graph* _plotGraph;
	int _priority;
	bool _plotCompleted = false;
	bool _discrete;

	virtual void BuildGraph() = 0;
	virtual string BuildSentence() = 0;
};
