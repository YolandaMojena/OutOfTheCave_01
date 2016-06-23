// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ontology/OEntity.h"
#include "Ontology/OEdification.h"
#include "Ontology/OOwnable.h"
#include "Ontology/ORelation.h"
#include "Ontology/OPersonality.h"
#include "Ontology/OOwnership.h"
#include "BasePlot.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "PlotTypes.h"
/**
 * 
 */
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
