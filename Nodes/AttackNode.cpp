// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "AttackNode.h"
#include "BasePlot.h"


AttackNode::~AttackNode(){}

void AttackNode::SetTarget(BasePlot* currentPlot)
{
	target = currentPlot->targetEntity;
}

void AttackNode::ExecuteActions(BasePlot* currentPlot, float deltaTime)
{
	currentPlot->plotEntity->SetCurrentIcon(iconPath.c_str());
	currentPlot->plotEntity->Attack(target, deltaTime);
}

bool AttackNode::NodeCompleted(BasePlot* currentPlot)
{
	return true;
}
