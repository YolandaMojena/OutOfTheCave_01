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
	AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation);
	~AttackPlot();

	void BuildSentence();
	bool ValidatePlot();

private:

	void BuildGraph();
	void ConsiderReactions();

	UOEntity* _targetEntity;
};


//DESTROY PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API DestroyPlot : public BasePlot
{
public:
	DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation);
	~DestroyPlot();

	void BuildSentence();
	bool ValidatePlot();

private:

	void BuildGraph();
	void ConsiderReactions();

	UOOwnable* _targetOwnable;
	UOEntity* _targetEntity;
};

//STAMPEDE
//**************************************************************************************

class OUTOFTHECAVE_01_API Stampede: public BasePlot
{
public:
	Stampede(ERace race, FVector spawnLocation, FVector targetLocation, int num);
	~Stampede();

	void BuildSentence();
	bool ValidatePlot();

private:

	void BuildGraph();
	void ConsiderReactions();

	ERace _raceToSpawn;
	FVector _spawnLocation;
	FVector _targetLocation;
	int _num;
};


//GATHER PLOT
//**************************************************************************************
/*
class OUTOFTHECAVE_01_API GatherPlot : public BasePlot
{
public:
	GatherPlot(UOEntity* plotEntity, UOOwnable* targetResource);
	~GatherPlot();

	void BuildSentence();

private:

	void BuildGraph();
	void ConsiderReactions();

	UOOwnable* _targetResource;
};*/

//STEAL PLOT
//**************************************************************************************
/*
class OUTOFTHECAVE_01_API StealPlot : public BasePlot
{
public:
	StealPlot(UOEntity* plotEntity, UOEntity* who, UOOwnable* target);
	~StealPlot();

	void BuildSentence();

private:

	void BuildGraph();
	void ConsiderReactions();

	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
};*/

//BUILD PLOT
//**************************************************************************************
/*
class OUTOFTHECAVE_01_API BuildPlot : public BasePlot
{
public:
	BuildPlot(UOEntity* plotEntity, UOEdification* target);
	~BuildPlot();

	void BuildSentence();

private:

	void BuildGraph();
	void ConsiderReactions();

	UOEdification* _targetEdification;
};*/


//GIVE PLOT
//**************************************************************************************
/*
class OUTOFTHECAVE_01_API GivePlot : public BasePlot
{
public:
	GivePlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what);
	~GivePlot();

	void BuildSentence();

private:

	void BuildGraph();
	void ConsiderReactions();

	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
};*/


