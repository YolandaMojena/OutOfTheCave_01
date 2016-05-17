// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Node.h"

class BasePlot;

class OUTOFTHECAVE_01_API AttackNode : public Node
{
public:

	using Node::Node;
	~AttackNode();

	void SetTarget(BasePlot* currentPlot);
	void ExecuteActions(BasePlot* currentPlot, float deltaTime);
	bool NodeCompleted(BasePlot* currentPlot);

	AEntity* target;
};
