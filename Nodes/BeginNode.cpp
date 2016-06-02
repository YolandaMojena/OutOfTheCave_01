// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BeginNode.h"
#include "BasePlot.h"

BeginNode::~BeginNode() {}

void BeginNode::ExecuteActions(BasePlot* currentPlot, float deltaTime)
{
}

bool BeginNode::NodeCompleted(BasePlot* currentPlot)
{
	return true;
}

void BeginNode::SetTarget(BasePlot* currentPlot)
{

}
