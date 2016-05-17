// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "RecruitNode.h"
#include "BasePlot.h"


RecruitNode::~RecruitNode() {}

void RecruitNode::SetTarget(BasePlot* currentPlot)
{
	target = currentPlot;
}

void RecruitNode::ExecuteActions(BasePlot* currentPlot, float deltaTime)
{
	currentPlot->plotEntity->SetCurrentIcon(iconPath.c_str());
	
}

bool RecruitNode::NodeCompleted(BasePlot* currentPlot)
{
	return true;
}
