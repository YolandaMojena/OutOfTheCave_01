// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "BasePlot.h"

using namespace std;

class UOEntity;
class UOOwnable;
class UOEdification;


//ATTACK PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API AttackPlot : public BasePlot
{
public:
	AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity);
	~AttackPlot();

private:

	void BuildGraph();
	string BuildSentence();
	void ConsiderReactions();

	UOEntity* _targetEntity;
};

//GATHER PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API GatherPlot : public BasePlot
{
public:
	GatherPlot(UOEntity* plotEntity, UOOwnable* targetResource);
	~GatherPlot();

private:

	void BuildGraph();
	string BuildSentence();
	void ConsiderReactions();

	UOOwnable* _targetResource;
};

//DESTROY PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API DestroyPlot : public BasePlot
{
public:
	DestroyPlot(UOEntity* plotEntity, UOOwnable* target);
	~DestroyPlot();

private:

	void BuildGraph();
	string BuildSentence();
	void ConsiderReactions();

	UOOwnable* _targetOwnable;
};

//STEAL PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API StealPlot : public BasePlot
{
public:
	StealPlot(UOEntity* plotEntity, UOEntity* who, UOOwnable* target);
	~StealPlot();

private:

	void BuildGraph();
	string BuildSentence();
	void ConsiderReactions();

	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
};

//BUILD PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API BuildPlot : public BasePlot
{
public:
	BuildPlot(UOEntity* plotEntity, UOEdification* target);
	~BuildPlot();

private:

	void BuildGraph();
	string BuildSentence();
	void ConsiderReactions();

	UOEdification* _targetEdification;
};


//GIVE PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API GivePlot : public BasePlot
{
public:
	GivePlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what);
	~GivePlot();

private:

	void BuildGraph();
	string BuildSentence();
	void ConsiderReactions();

	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
};


