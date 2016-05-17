// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "GatherNode.h"
#include "BasePlot.h"


GatherNode::~GatherNode() {}

void GatherNode::SetTarget(BasePlot* currentPlot)
{
	target = currentPlot->targetThing;
}

void GatherNode::ExecuteActions(BasePlot* currentPlot, float deltaTime)
{
	currentPlot->plotEntity->SetCurrentIcon(iconPath.c_str());
	currentPlot->plotEntity->Gather(target, deltaTime);


}

bool GatherNode::NodeCompleted(BasePlot* currentPlot)
{
	return true;
}
