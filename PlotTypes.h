// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "BasePlot.h"

using namespace std;

class UOEntity;
class UOOwnable;

/**
 * 
 */
class OUTOFTHECAVE_01_API AttackPlot : public BasePlot
{
public:
	AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity);
	~AttackPlot();

	void GatherTargets();
	void ConsiderReactions();


private:

	void BuildGraph();
	string BuildSentence();

	UOEntity* _targetEntity;
};

class OUTOFTHECAVE_01_API GatherPlot : public BasePlot
{
public:
	GatherPlot(UOEntity* plotEntity, UOOwnable* targetResource);
	~GatherPlot();

	void GatherTargets();
	void ConsiderReactions();


private:

	void BuildGraph();
	string BuildSentence();

	UOOwnable* _targetResource;
};

/*
class OUTOFTHECAVE_01_API StealPlot : public BasePlot
{
public:
	StealPlot();
	~StealPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API KidnapPlot : public BasePlot
{
public:
	KidnapPlot();
	~KidnapPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API RescuePlot : public BasePlot
{
public:
	RescuePlot();
	~RescuePlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API CelebratePlot : public BasePlot
{
public:
	CelebratePlot();
	~CelebratePlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API AmbushPlot : public BasePlot
{
public:
	AmbushPlot();
	~AmbushPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API GiftPlot : public BasePlot
{
public:
	GiftPlot();
	~GiftPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API BuildPlot : public BasePlot
{
public:
	BuildPlot();
	~BuildPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API DestroyPlot : public BasePlot
{
public:
	DestroyPlot();
	~DestroyPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API ExplodePlot : public BasePlot
{
public:
	ExplodePlot();
	~ExplodePlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API DefendPlot : public BasePlot
{
public:
	DefendPlot();
	~DefendPlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API AvalanchePlot : public BasePlot
{
public:
	AvalanchePlot();
	~AvalanchePlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};

class OUTOFTHECAVE_01_API NaturesRevengePlot : public BasePlot
{
public:
	NaturesRevengePlot();
	~NaturesRevengePlot();

	void GatherTargets(vector<AEntity*> candidates);
	void BuildSentence();

};*/

