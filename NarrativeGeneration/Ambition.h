// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class BasePlot;
class UOEntity;
class APlotGenerator;

enum  TypeOfAmbition : uint8 {

	possessions,
	extermination,
	notoriety,
	appreciation,
	fear,
	friendTroll,
	noAmbition
};

class OUTOFTHECAVE_01_API Ambition
{
public:

	Ambition();
	Ambition(APlotGenerator* plotGenerator, UOEntity* troll);
	~Ambition();

	BasePlot* GenerateAmbitionForEntity(UOEntity* entity);

private:

	BasePlot* GetPosessionsAmbition(UOEntity* entity);
	BasePlot* ExterminateAmbition(UOEntity* entity);
	BasePlot* BecomeNotoriousAmbition(UOEntity* entity);
	BasePlot* BecomeAppreciatedAmbition(UOEntity* entity);
	BasePlot* BecomeFearedAmbition(UOEntity* entity);
	BasePlot* FriendTrollAmbition(UOEntity* entity);

	APlotGenerator* _plotGenerator;
	UOEntity* _troll;
};
