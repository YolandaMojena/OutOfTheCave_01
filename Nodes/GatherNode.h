// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Node.h"

class BasePlot;

class OUTOFTHECAVE_01_API GatherNode : public Node
{
public:

	using Node::Node;
	~GatherNode();

	void ExecuteActions(BasePlot* currentPlot, float deltaTime);
	bool NodeCompleted(BasePlot* currentPlot);
	void SetTarget(BasePlot* currentPlot);

	AThing* target;
};
