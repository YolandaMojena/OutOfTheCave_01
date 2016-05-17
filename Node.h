// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <algorithm> 
#include <unordered_map>
#include "Entity.h"
#include "Thing.h"
#include "VenturLocation.h"

using namespace std;

class BasePlot;

class OUTOFTHECAVE_01_API Node
{
public:

	struct Arc {

		Arc(string from, string to, string cond);

		string parent;
		string adj;
		string condition;
	};

	Node(string id, string icon_path);

	Node();
	~Node();

	bool GetIsVisited();
	void SetIsVisited(bool value);

	virtual bool NodeCompleted(BasePlot* currentPlot) = 0;
	virtual void ExecuteActions(BasePlot* currentPlot, float deltaTime) = 0;
	virtual void SetTarget(BasePlot* currentPlot) = 0;

	vector<Arc> neighbors;
	string nodeName;
	string iconPath;

private:
	
	bool _visited = false;
	
};
