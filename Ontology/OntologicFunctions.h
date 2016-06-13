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
	
	int HyperbolicGrowth(int value);
	int LinearGrowth(int x);
	int ExponentialGrowth(int x);
	int HyperbolicGrowth(int value, int magnitude);
	int LinearGrowth(int x, int m);
	int ExponentialGrowth(int x, int m);

	int HyperbolicDecay(int x, int m);
	int LinearDecay(int x, int m);
	int ExponentialDecay(int x, int m);

	int HyperbolicEqualization(int x, int m);
	int LinearEqualization(int x, int m);
	int ExponentialEqualization(int x, int m);

	int ThresholdValue(int x, int t);
	int ThresholdValue(int x);
	int BotThresholdValue(int x);
};
