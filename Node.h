// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <string>

using namespace std;

class BasePlot;
class Graph;
class UOEntity;
class UOOwnable;
class UOEdification;
class UOGrabbable;

class OUTOFTHECAVE_01_API Node
{

public:
	Node();
	~Node();

	class NBlackboard {
	public:

		UOEntity* entityA;
		UOEntity* entityB;
		UOOwnable* ownable;
		UOEdification* edification;
		UOGrabbable* grabbable;
		FVector* position;
	};

	void SetGraph(Graph* g);
	void RewriteNode(vector<Node*> nodes);
	void ExecuteTask();
	void TaskCompleted(bool completedOk);

	// Allows nullptr if a value is not required
	void PopulateBlackboard(UOEntity* entityA = nullptr, UOEntity* entityB = nullptr, UOOwnable* ownable = nullptr, UOEdification* edification = nullptr, UOGrabbable* grabbable = nullptr, FVector* position = nullptr);

	NBlackboard nBlackboard;
	string name;
	vector<Node*> nextNodes;

private:
	Graph* _graph;
	
};
