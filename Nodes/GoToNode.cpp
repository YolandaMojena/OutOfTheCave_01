// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "GoToNode.h"
#include "BasePlot.h"

GoToNode::~GoToNode() {}

void GoToNode::SetTarget(BasePlot* currentPlot)
{

}

void GoToNode::ExecuteActions(BasePlot* currentPlot, float deltaTime)
{

}

bool GoToNode::NodeCompleted(BasePlot* currentPlot)
{
	if(FVector(currentPlot->plotEntity->GetOwner()->GetActorLocation().X  - target->worldPoint.X, currentPlot->plotEntity->GetOwner()->GetActorLocation().Y - target->worldPoint.Y, 0).Size() < 50)
		return true;
	else return false;
}
