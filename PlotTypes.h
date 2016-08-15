// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "NarrativeGeneration/Ambition.h"
#include "BasePlot.h"

using namespace std;

class UOEntity;
class UOOwnable;
class UOEdification;
class APlotGenerator;


//ATTACK PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API AttackPlot : public BasePlot
{
public:
	AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation);
	AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition);
	~AttackPlot();

	void BuildSentence();
	UOEntity* GetTargetEntity();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();
	UOEntity* _targetEntity;
};


//DESTROY PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API DestroyPlot : public BasePlot
{
public:
	DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOEdification* targetEdification, UItem* motivation);
	DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOEdification* targetEdification, TypeOfAmbition ambition);
	~DestroyPlot();

	void BuildSentence();
	UOEntity* GetTargetEntity();
	UOEdification* GetTargetEdification();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEdification* _targetEdification;
	UOEntity* _targetEntity;
};


//BUILD PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API BuildPlot : public BasePlot
{
public:
	BuildPlot(UOEntity* plotEntity, UOEdification* target, UItem* motivation);
	BuildPlot(UOEntity* plotEntity, UOEdification* target, TypeOfAmbition ambition);
	~BuildPlot();

	void BuildSentence();
	UOEdification* GetTargetEdification();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEdification* _targetEdification;
};


//STEAL PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API StealPlot : public BasePlot
{
public:
	StealPlot(UOEntity* plotEntity, UOEntity* who, UOOwnable* target, UItem* motivation);
	StealPlot(UOEntity* plotEntity, UOEntity* who, UOOwnable* target, TypeOfAmbition ambition);
	~StealPlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	UOOwnable* GetTargetOwnable();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
};

//COLLECT PLOT = GET
//**************************************************************************************

class OUTOFTHECAVE_01_API GetPlot : public BasePlot
{
public:
	GetPlot(UOEntity* plotEntity, UOOwnable* target, UItem* motivation);
	GetPlot(UOEntity* plotEntity, UOOwnable* target, TypeOfAmbition ambition);
	~GetPlot();

	void BuildSentence();
	void InitPlot();
	UOOwnable* GetTargetOwnable();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOOwnable* _targetOwnable;
	vector<UOEntity*> _owners;
};



//AMBUSH
//**************************************************************************************

class OUTOFTHECAVE_01_API AmbushPlot : public BasePlot
{
public:
	AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* target, UItem* motivation);
	AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* target, TypeOfAmbition ambition);
	~AmbushPlot();

	void BuildSentence();
	UOEntity* GetTargetEntity();
	UItem* GetTarget();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEntity* _targetEntity;
	UItem* _target;
};


//HELP PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API HelpPlot : public BasePlot
{
public:
	HelpPlot(UOEntity* plotEntity, UOEntity* target, UItem* motivation);
	HelpPlot(UOEntity* plotEntity, UOEntity* target, TypeOfAmbition ambition);
	~HelpPlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEntity* _targetEntity;
};

//GIVE PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API GivePlot : public BasePlot
{
public:
	GivePlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what, UItem* motivation);
	GivePlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what, TypeOfAmbition ambition);
	~GivePlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	UOOwnable* GetTargetOwnable();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
};

//DEFEND PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API DefendPlot : public BasePlot
{
public:
	DefendPlot(UOEntity* plotEntity, UOEntity* against, UItem* target, UItem* motivation);
	//DefendPlot(UOEntity* plotEntity, UOEntity* target, TypeOfAmbition ambition);
	~DefendPlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	UOEntity* GetTargetToDefend();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UItem* _target;
	UOEntity* _against;
};


//WAR
//**************************************************************************************

class OUTOFTHECAVE_01_API WarPlot : public BasePlot
{
public:
	WarPlot(UOEntity* plotEntity);
	~WarPlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();
	UOEntity* _targetEntity;
};




//STAMPEDE
//**************************************************************************************

class OUTOFTHECAVE_01_API Stampede : public BasePlot
{
public:
	Stampede(ERace race, FVector spawnLocation, FVector targetLocation, float num, APlotGenerator* plotGenerator);
	Stampede(ERace race, FVector spawnLocation, UOEntity* targetActor, float num, APlotGenerator* plotGenerator);
	~Stampede();

	void BuildSentence();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();
	ERace _race;
	FString _raceString;
	FVector _spawnLocation;
	FVector _targetLocation;
	UOEntity* _targetActor;
	APlotGenerator* _plotGenerator;
	float _num;
	vector<UOEntity*> _heard;
};




