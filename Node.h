// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <string>

#include "Ontology/OEntity.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OEdification.h"
#include "Ontology/OGrabbable.h"

using namespace std;

class BasePlot;
class Graph;

class OUTOFTHECAVE_01_API Node
{

public:
	Node();
	~Node();

	class NBlackboard {
	public:
		//NBlackboard();
		//~NBlackboard();

		UOEntity* entityA;
		UOEntity* entityB;
		UOOwnable* ownable;
		UOEdification* edification;
		UOGrabbable* grabbable;
		FVector* position;
	};

	NBlackboard nBlackboard;

	void SetGraph(Graph* g);

	string name;

	vector<Node*> nextNodes;

	void RewriteNode(vector<Node*> nodes);

	void ExecuteTask();
	void TaskCompleted(bool completedOk);

private:
	Graph* _graph;
	
};
