// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BasePlot.h"
#include "PlotTypes.h"
#include "Ontology/OEntity.h"
#include "Ontology/OEdification.h"
#include "Ontology/OOwnable.h"
#include "Ontology/ORelation.h"
#include "Ontology/OPersonality.h"
#include "Ontology/OOwnership.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "Ambition.h"

Ambition::Ambition(){}

Ambition::Ambition(APlotGenerator* plotGenerator, UOEntity* troll) {

	_plotGenerator = plotGenerator;
	_troll = troll;
}

Ambition::~Ambition(){}

BasePlot * Ambition::GenerateAmbitionForEntity(UOEntity * entity)
{
	// If no ambition, generate from personality
	if (((UOCivilian*)entity)->GetAmbition() == TypeOfAmbition::noAmbition) {

		OPersonality* entPersonality = entity->GetPersonality();
		vector<BasePlot*> candidates;

		//POSSESSIONS
	//	if (entPersonality->GetMaterialist() > 50 && entPersonality->GetCurious() > 50)
			candidates.push_back(GetPosessionsAmbition(entity));

		/*//EXTERMINATE
		if (entPersonality->GetBraveness() > 50 && entPersonality->GetAggressiveness() > 50 && entPersonality->GetSocial() < 50)
			candidates.push_back(ExterminateAmbition(entity));

		//NOTORIETY
		if (entPersonality->GetAggressiveness() > 50 && entPersonality->GetSocial() > 50)
			candidates.push_back(BecomeNotoriousAmbition(entity));

		//BE APPRECIATED
		if (entPersonality->GetKindness() > 50 && entPersonality->GetSocial() > 50)
			candidates.push_back(BecomeAppreciatedAmbition(entity));

		//BE FEARED
		if (entPersonality->GetBraveness() < 50 && entPersonality->GetAggressiveness() > 50)
			candidates.push_back(BecomeFearedAmbition(entity));

		//FRIEND TROLL
		if (entPersonality->GetSocial() < 50 && entPersonality->GetCurious() > 50)
			candidates.push_back(FriendTrollAmbition(entity));*/

		if (candidates.size() > 0) {
			BasePlot* ambitionPlot = candidates[rand() % candidates.size()];
			((UOCivilian*)entity)->SetAmbition(ambitionPlot->GetPlotTypeOfAmbition());
			return ambitionPlot;
		}
	}
	else {

		switch (((UOCivilian*)entity)->GetAmbition()) {

		case TypeOfAmbition::appreciation:
			return nullptr;

		case TypeOfAmbition::extermination:
			return ExterminateAmbition(entity);

		case TypeOfAmbition::notoriety:
			return BecomeNotoriousAmbition(entity);

		case TypeOfAmbition::fear:
			return BecomeFearedAmbition(entity);

		case TypeOfAmbition::friendTroll:
			return FriendTrollAmbition(entity);

		case TypeOfAmbition::possessions:
			return GetPosessionsAmbition(entity);
		}
	}	

	return nullptr;
}

// Materialistic ambition
//Check if owned
//If not owned, check if it's in someone's inventory
//If it is, steal, if not, get
BasePlot * Ambition::GetPosessionsAmbition(UOEntity * entity)
{
	UOOwnable* valuableToOwn = nullptr;
	vector<UOOwnable*> valuables;
	valuables = _plotGenerator->GetValuables();

	for (UOOwnable* o : valuables) {
		if (!entity->GetOwnershipWith(o)) {
			valuableToOwn = o;
			break;
		}	
	}

	
	/*if (valuableToOwn->GetOwners().size() > 0) {

		for (UOEntity* e : valuableToOwn->GetOwners()) {
			bool savedInInventory = false;
			for (UOOwnable* o : e->GetInventory()) {
				if (o == valuableToOwn) {
					savedInInventory = true;
					break;
				}
			}
			if (savedInInventory)
				return new StealPlot(entity, e, valuableToOwn, TypeOfAmbition::possessions);
		}
	}*/
	if(valuableToOwn)
	return new GetPlot(entity, valuableToOwn, TypeOfAmbition::possessions);
	else return nullptr;
}

//Ambition based on pure hate against a race
BasePlot * Ambition::ExterminateAmbition(UOEntity * entity)
{
	ERace targetRace;
	if (entity->GetRace() == ERace::R_Human)
		targetRace = ERace::R_Goblin;
	else targetRace = ERace::R_Human;

	vector<UOEntity*> targetEntities = _plotGenerator->GetNotoriousEntitiesByRace(targetRace);

	if (entity->GetPersonality()->GetAstute() > 50)
		return new AmbushPlot(entity, targetEntities[targetEntities.size() - 1], TypeOfAmbition::extermination);
	else
		return new AttackPlot(entity, targetEntities[targetEntities.size() - 1], TypeOfAmbition::extermination);
}

//Ambition based on two paths for becoming notorious: killing to ascend or helping those who already are notorious
BasePlot * Ambition::BecomeNotoriousAmbition(UOEntity * entity)
{
	ERace targetRace;
	if (entity->GetRace() == ERace::R_Human)
		targetRace = ERace::R_Goblin;
	else targetRace = ERace::R_Human;

	vector<UOEntity*> targetEntities = _plotGenerator->GetNotoriousEntitiesByRace(targetRace);

	if(entity->GetPersonality()->GetAggressiveness() > entity->GetPersonality()->GetKindness())
		return new AttackPlot(entity, targetEntities[targetEntities.size() - 1], TypeOfAmbition::notoriety);
	else
		return new HelpPlot(entity, targetEntities[0], TypeOfAmbition::notoriety);
}

BasePlot * Ambition::BecomeAppreciatedAmbition(UOEntity * entity)
{
	BasePlot* ambitionPlot = nullptr;
	return ambitionPlot;
}

// Destructive ambition, destroy homes from those who are notorious
BasePlot * Ambition::BecomeFearedAmbition(UOEntity * entity)
{
	ERace targetRace;
	if (entity->GetRace() == ERace::R_Human)
		targetRace = ERace::R_Goblin;
	else targetRace = ERace::R_Human;

	vector<UOEntity*> targetEntities = _plotGenerator->GetNotoriousEntitiesByRace(targetRace);

	return new DestroyPlot(entity, targetEntities[0], TypeOfAmbition::fear);
}

// Just a simple ambition: being closer to the troll
BasePlot * Ambition::FriendTrollAmbition(UOEntity * entity)
{
	return new HelpPlot(entity, _troll, TypeOfAmbition::friendTroll);
}
