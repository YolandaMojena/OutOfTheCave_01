// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include "Graph.h"
#include "Ontology/OCivilian.h"
#include "StringCollection.h"
#include "VenturLocation.h"

using namespace std;

class OUTOFTHECAVE_01_API BasePlot
{
public:

	BasePlot(); // Constructor used for wold plots that don't involve a specific entity
	BasePlot(UOCivilian* plotEntity);
	~BasePlot();

	enum TypeOfPlot {
		aggressive, thankful, preventive, possesive, resources, world
	};

	virtual void GatherTargets() = 0;
	virtual string BuildSentence() = 0;
	virtual void ConsiderReactions() = 0;
	
	void ExecutePlot(float deltaTime);
	void BuildGraph(vector<Node*> nodes, vector<Node::Arc> arcs);
	void PrintSentence();

	bool Compare(BasePlot* thisPlot, BasePlot* otherPlot);

	TypeOfPlot plotTypes;
	BasePlot* additiveReactions;
	BasePlot* negativeReactions;
	UOCivilian* plotEntity;
	vector<UOCivilian*> involvedInPlot;
	StringCollection strings;

protected:
	
	string _name, _description, _sentence;
	Graph _plotGraph;
	int _priority;
	Node* _currentNode = nullptr;
	bool _plotCompleted = false;
};
