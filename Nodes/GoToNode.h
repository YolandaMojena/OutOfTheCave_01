// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Node.h"


class BasePlot;

class OUTOFTHECAVE_01_API GoToNode : public Node
{
public:

	using Node::Node;
	~GoToNode();

	VenturLocation* target;

	void ExecuteActions(BasePlot* currentPlot, float deltaTime);
	bool NodeCompleted(BasePlot* currentPlot);
	void SetTarget(BasePlot* currentPlot);
};
