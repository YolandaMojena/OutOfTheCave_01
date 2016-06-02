// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Node.h"

class BasePlot;

class OUTOFTHECAVE_01_API BeginNode : public Node
{
public:

	using Node::Node;
	~BeginNode();

	void ExecuteActions(BasePlot* currentPlot, float deltaTime);
	bool NodeCompleted(BasePlot* currentPlot);
	void SetTarget(BasePlot* currentPlot);


private:

	bool _alliesVisited = false;
	bool _beganRecluting = false;
	int _entityNum;
};
