// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OntologicFunctions.h"

OntologicFunctions::OntologicFunctions()
{
}

OntologicFunctions::~OntologicFunctions()
{
}


int OntologicFunctions::GetAffordance(AffordableUse au, UItem* i) {
	switch (au) {
	case AffordableUse::cultivator:
		return UseAsCultivator(i);
	case AffordableUse::mine:
		return UseForMining(i);
	case AffordableUse::weapon:
		return UseAsWeapon(i);
	case AffordableUse::build:
		return UseForBuilding(i);
	default:
		return 0;
	}
}

int OntologicFunctions::UseAsWeapon(UItem* i) {
	return i->GetToughness() * (i->GetAngularMomentumTransmission() * i->GetMass() + i->GetAngularInertia() / i->GetMass()) * (i->GetElongation() + i->GetEdgeSharpness() * i->GetEdgeLength() + i->GetSpiky());
}
int OntologicFunctions::Pierce(UItem* i) {
	return i->GetToughness() * (i->GetEdgeSharpness() * i->GetEdgeLength() + i->GetSpiky() * i->GetSpikes());
}
int OntologicFunctions::UseAsCultivator(UItem* i){
	return i->GetToughness() * i->GetElongation() * (i->GetEdgeSharpness() / i->GetEdgeLength() + i->GetSpikes()) / i->GetMass();
}
int OntologicFunctions::UseForMining(UItem* i) {
	return i->GetToughness() * i->GetSpiky() * i->GetAngularMomentumTransmission() * i->GetMass();
}
int OntologicFunctions::UseForBuilding(UItem* i) {
	return i->GetToughness() * i->GetAngularMomentumTransmission() / i->GetMaxLength() / i->GetMass();
}



int OntologicFunctions::ReduceWeight(int value) {
	return value / 2;
}
int OntologicFunctions::AmplifyWeight(int value) {
	return value * 2;
}