// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include "Graph.h"
#include "Entity.h"
#include "Thing.h"
#include "VenturLocation.h"
#include "Node.h"
#include "Entity.h"
#include "Thing.h"
#include "StringCollection.h"
#include "VenturLocation.h"

using namespace std;

class OUTOFTHECAVE_01_API BasePlot
{
public:
	BasePlot();
	~BasePlot();

	virtual void GatherTargets(vector<AEntity*> candidates) = 0;
	virtual void BuildSentence() = 0;
	
	void ExecutePlot(float deltaTime);
	void BuildGraph(vector<Node*> nodes, vector<Node::Arc> arcs);
	
	string name, description, sentence;
	Graph plotGraph;

	AEntity* plotEntity;
	vector<AEntity*> involvedInPlot;

	VenturLocation* targetLocation;
	AThing* targetThing;
	AEntity* targetEntity;

	StringCollection strings;

	Node* currentNode = nullptr;

	bool plotCompleted = false;
};
