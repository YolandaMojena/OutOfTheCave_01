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
	AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, FString motivationName, FString motivationRace);
	AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition);
	~AttackPlot();

	void BuildSentence();
	UOEntity* GetTargetEntity();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();
	UOEntity* _targetEntity = nullptr;
};


//DESTROY PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API DestroyPlot : public BasePlot
{
public:
	DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOEdification* targetEdification, FString motivationName, FString motivationRace);
	DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOEdification* targetEdification, TypeOfAmbition ambition);
	~DestroyPlot();

	void BuildSentence();
	UOEntity* GetTargetEntity();
	UOEdification* GetTargetEdification();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEdification* _targetEdification = nullptr;
	UOEntity* _targetEntity = nullptr;
};


//BUILD PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API BuildPlot : public BasePlot
{
public:
	BuildPlot(UOEntity* plotEntity, UOEdification* target, FString motivationName, FString motivationRace);
	BuildPlot(UOEntity* plotEntity, UOEdification* target, TypeOfAmbition ambition);
	~BuildPlot();

	void BuildSentence();
	UOEdification* GetTargetEdification();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEdification* _targetEdification = nullptr;
};


//COLLECT PLOT = GET
//**************************************************************************************

class OUTOFTHECAVE_01_API GetPlot : public BasePlot
{
public:
	GetPlot(UOEntity* plotEntity, UOOwnable* target, FString motivationName, FString motivationRace);
	GetPlot(UOEntity* plotEntity, UOOwnable* target, TypeOfAmbition ambition);
	~GetPlot();

	void BuildSentence();
	void InitPlot();
	UOOwnable* GetTargetOwnable();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOOwnable* _targetOwnable = nullptr;
	vector<UOEntity*> _owners;
};



/*AMBUSH
//**************************************************************************************

class OUTOFTHECAVE_01_API AmbushPlot : public BasePlot
{
public:
	AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, FString motivationName, FString motivationRace);
	AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition);
	~AmbushPlot();

	void BuildSentence();
	UOEntity* GetTargetEntity();
	void InitPlot();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEntity* _targetEntity;
	UItem* _target;
};*/


//HELP PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API HelpPlot : public BasePlot
{
public:
	HelpPlot(UOEntity* plotEntity, UOEntity* target, FString motivationName, FString motivationRace);
	HelpPlot(UOEntity* plotEntity, UOEntity* target, TypeOfAmbition ambition);
	~HelpPlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOEntity* _targetEntity = nullptr;
};

//DEFEND PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API DefendPlot : public BasePlot
{
public:
	DefendPlot(UOEntity* plotEntity, UOEntity* against, UItem* target, FString motivationName, FString motivationRace);
	//DefendPlot(UOEntity* plotEntity, UOEntity* target, TypeOfAmbition ambition);
	~DefendPlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	BasePlot* ConsiderReactions();
	UItem* GetTarget();

private:

	void BuildGraph();
	UOEntity* _against = nullptr;
	UItem* _target = nullptr;
};

//STEAL PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API StealPlot : public BasePlot
{
public:
	StealPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOOwnable* targetOwnable, FString motivationName, FString motivationRace);
	StealPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOOwnable* targetOwnable, TypeOfAmbition ambition);
	~StealPlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	UOOwnable* GetTargetOwnable();

	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOOwnable* _targetOwnable = nullptr;
	UOEntity* _targetEntity = nullptr;
};

//GIVE PLOT
//**************************************************************************************

class OUTOFTHECAVE_01_API GivePlot : public BasePlot
{
public:
	GivePlot(UOEntity* plotEntity, UOEntity* targetEntity, UOOwnable* targetOwnable, FString motivationName, FString motivationRace);
	GivePlot(UOEntity* plotEntity, UOEntity* targetEntity, UOOwnable* targetOwnable, TypeOfAmbition ambition);
	~GivePlot();

	void BuildSentence();
	void InitPlot();
	UOEntity* GetTargetEntity();
	UOOwnable* GetTargetOwnable();

	BasePlot* ConsiderReactions();

private:

	void BuildGraph();

	UOOwnable* _targetOwnable = nullptr;
	UOEntity* _targetEntity = nullptr;
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
	UOEntity* _targetEntity = nullptr;
};




//STAMPEDE
//**************************************************************************************

class OUTOFTHECAVE_01_API Stampede : public BasePlot
{
public:
	Stampede(ERace race, FVector spawnLocation, APlotGenerator* plotGenerator);
	Stampede(ERace race, FVector spawnLocation, UOEntity* targetActor, APlotGenerator* plotGenerator);
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
	UOEntity* _targetActor = nullptr;
	APlotGenerator* _plotGenerator = nullptr;
	float _num;
	vector<UOEntity*> _heard;
};




