// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Nodes/GoToNode.h"
#include "Nodes/BeginNode.h"
#include "Nodes/AttackNode.h"
#include "Nodes/GatherNode.h"
#include "BasePlot.h"

/**
 * 
 */
class OUTOFTHECAVE_01_API AttackPlot : public BasePlot
{
public:
	AttackPlot();
	~AttackPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API GatherPlot : public BasePlot
{
public:
	GatherPlot();
	~GatherPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API GoToPlot : public BasePlot
{
public:
	GoToPlot();
	~GoToPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};