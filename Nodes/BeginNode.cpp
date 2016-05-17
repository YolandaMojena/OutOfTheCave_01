// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BeginNode.h"
#include "BasePlot.h"

BeginNode::~BeginNode() {}

void BeginNode::ExecuteActions(BasePlot* currentPlot, float deltaTime)
{
	currentPlot->plotEntity->SetCurrentIcon(iconPath.c_str());
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, "Hi, I am the main entity: " + currentPlot->plotEntity->GetActorLabel());
}

bool BeginNode::NodeCompleted(BasePlot* currentPlot)
{
	return true;
}

void BeginNode::SetTarget(BasePlot* currentPlot)
{
	target = currentPlot->plotEntity;
}
