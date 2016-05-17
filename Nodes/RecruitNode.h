// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Node.h"

class BasePlot;

class OUTOFTHECAVE_01_API RecruitNode : public Node
{
public:

	enum KindOfRecruit {
		equal, troll
	};
	
	RecruitNode(KindOfRecruit kind, string id, string icon_path) : Node(id, icon_path) {};
	~RecruitNode();

	BasePlot* target;

	void ExecuteActions(BasePlot* currentPlot, float deltaTime);
	bool NodeCompleted(BasePlot* currentPlot);
	void SetTarget(BasePlot* currentPlot);

private:


};
