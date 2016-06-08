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

enum NodeType {
	branch, goTo, wait, interact, enter, attack, get, grab, steal, destroy, celebrate, give, build, askForHelp
};

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
		string arquetypeObject;
		UOEdification* edification;
		UOGrabbable* grabbable;
		FVector position;
		vector<float> daytimes;
		bool branch;
	};

	void SetGraph(Graph* g);
	void RewriteNode(vector<Node*> nodes);
	void ExecuteTask();
	void TaskCompleted(bool completedOk);

	// Allows nullptr if a value is not required
	void PopulateBlackboard(UOEntity* entityA = nullptr, UOEntity* entityB = nullptr, UOOwnable* ownable = nullptr, UOEdification* edification = nullptr, UOGrabbable* grabbable = nullptr);
	void PopulateBlackboard(FVector position, UOEntity* entityA = nullptr, UOEntity* entityB = nullptr, UOOwnable* ownable = nullptr, UOEdification* edification = nullptr, UOGrabbable* grabbable = nullptr);
	void SetIconPath(string path);

	void SetEntityA(UOEntity* e); //TBI
	void SetEntityB(UOEntity* e); //TBI
	void SetOwnable(UOOwnable* o); //TBI
	// [...]
	void SetArquetypeObject(string s);

	NBlackboard nBlackboard;
	string name;
	vector<Node*> nextNodes;

	void SetNodeType(NodeType n);

private:
	Graph* _graph;
	string _iconPath;
	NodeType _nodeType;
};
