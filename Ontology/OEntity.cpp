// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OEntity.h"



UOEntity::UOEntity() {
	
}

UOEntity::UOEntity(OPersonality* personality) {
	_personality = personality;
}


vector<ORelation*> UOEntity::GetRelationships() {
	return _relationships;
}
vector<OOwnership*> UOEntity::GetPossessions() {
	return _possessions;
}
vector<OTerritory*> UOEntity::GetTerritories() {
	return _landlord;
}

void UOEntity::AddRelationship(ORelation* newRelation) {
	_relationships.push_back(newRelation);
}
void UOEntity::AddPossession(OOwnership* newPossession) {
	_possessions.push_back(newPossession);
}
void UOEntity::AddTerritory(OTerritory* newTerritory) {
	_landlord.push_back(newTerritory);
}