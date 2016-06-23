// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BasePlot.h"
#include "Ambition.h"

Ambition::Ambition(){}

Ambition::Ambition(APlotGenerator* plotGenerator, UOEntity* troll) {

	_plotGenerator = plotGenerator;
	_troll = troll;
}

Ambition::~Ambition(){}

BasePlot * Ambition::GenerateAmbitionForEntity(UOEntity * entity)
{
	OPersonality* entPersonality = entity->GetPersonality();
	vector<BasePlot*> candidates;

	//POSSESSIONS
	if (entPersonality->GetMaterialist() > 50  && entPersonality->GetCurious() > 50)
		candidates.push_back(GetPosessionsAmbition(entity));

	//EXTERMINATE
	if (entPersonality->GetBraveness() > 50  && entPersonality->GetAggressiveness() > 50 && entPersonality->GetSocial() < 50)
		candidates.push_back(ExterminateAmbition(entity));
	
	//NOTORIETY
	if (entPersonality->GetAggressiveness() > 50 && entPersonality->GetSocial() > 50)
		candidates.push_back(BecomeNotoriousAmbition(entity));
	
	//BE APPRECIATED
	if (entPersonality->GetKindness() > 50  && entPersonality->GetSocial() > 50)
		candidates.push_back(BecomeAppreciatedAmbition(entity));

	//BE FEARED
	if (entPersonality->GetBraveness() < 50 && entPersonality->GetAggressiveness() > 50)
		candidates.push_back(BecomeFearedAmbition(entity));

	//FRIEND TROLL
	if (entPersonality->GetSocial() < 50 && entPersonality->GetCurious() > 50)
		candidates.push_back(FriendTrollAmbition(entity));


	if (candidates.size() > 0) 
		return candidates[rand() % candidates.size()];
	else
		return nullptr;
}

BasePlot * Ambition::GetPosessionsAmbition(UOEntity * entity)
{
	BasePlot* ambitionPlot = nullptr;
	UOOwnable* valuableToOwn = nullptr;
	vector<UOOwnable*> valuables;
	//valuables = _plotGenerator->GetValuables();

	for (UOOwnable* o : valuables) {
		if (!entity->GetOwnershipWith(o)) {
			valuableToOwn = o;
			break;
		}	
	}

	if (valuableToOwn->GetOwners().size() > 0) {

		for (UOEntity* o : valuableToOwn->GetOwners()) {

			// Check inventory
			// If it has to be stolen
			// ambitionPlot = new StealPlot(entity, o, valuableToOwn, TypeOfAmbition::possessive);
			// If not 
			// ambitionPlot = new GetPlot(entity, valuableToOwn, entity);
		}
	}
	// else ambitionPlot = new GetPlot(entity, valuableToOwn, entity);


	return ambitionPlot;
}

BasePlot * Ambition::ExterminateAmbition(UOEntity * entity)
{
	//check race
	//check notorious entities from that race
	BasePlot* ambitionPlot = nullptr;


	return ambitionPlot;
}

BasePlot * Ambition::BecomeNotoriousAmbition(UOEntity * entity)
{
	BasePlot* ambitionPlot = nullptr;
	return ambitionPlot;
}

BasePlot * Ambition::BecomeAppreciatedAmbition(UOEntity * entity)
{
	BasePlot* ambitionPlot = nullptr;
	return ambitionPlot;
}

BasePlot * Ambition::BecomeFearedAmbition(UOEntity * entity)
{
	BasePlot* ambitionPlot = nullptr;
	return ambitionPlot;
}

BasePlot * Ambition::FriendTrollAmbition(UOEntity * entity)
{
	BasePlot* ambitionPlot = nullptr;
	return ambitionPlot;
}
