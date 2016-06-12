// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ontology/Item.h"

/**
 * 
 */
class OUTOFTHECAVE_01_API OntologicFunctions
{
public:
	OntologicFunctions();
	~OntologicFunctions();

	enum AffordableUse {
		weapon, cultivator, mine, build
	};

	int GetAffordance(AffordableUse au, UItem* i);

	int UseAsWeapon(UItem* i);
	int Pierce(UItem* i);
	int UseAsCultivator(UItem* i);
	int UseForMining(UItem* i);
	int UseForBuilding(UItem* i);

private:
	int ReduceWeight(int value);
	int AmplifyWeight(int value);
};
