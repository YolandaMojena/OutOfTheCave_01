// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "Ontology/OEntity.h"

UOEntity::UOEntity() {
	
}

UOEntity::UOEntity(OPersonality* personality) {
	_personality = personality;
}

void UOEntity::BeginPlay() {

	Super::BeginPlay();
	
	for (TObjectIterator<APlotGenerator> Itr; Itr; ++Itr)
		plotGenerator = *Itr;
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

OPersonality * UOEntity::GetPersonality(){
	return _personality;
}

bool UOEntity::GetIsDead() {
	return _isDead;
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

ORelation* UOEntity::GetRelationWith(UOEntity * other)
{
	for (int i = 0; i < _relationships.size(); i++) {
		if (_relationships[i]->GetEntity() == other)
			return _relationships[i];
	}

	return nullptr;
}

OOwnership* UOEntity::GetOwnershipWith(UOOwnable * other)
{
	for (int i = 0; i < _possessions.size(); i++) {
		if (_possessions[i]->GetItem() == other)
			return _possessions[i];
	}

	return nullptr;
}

void UOEntity::ReceiveDamage(float damage, UOEntity * damager)
{
	if (!_isDead) {

		_integrity -= damage;
		_attacker = damager;

		if (_integrity < MIN_INTEGRITY) {
			_isDead = true;
			IHaveBeenKilledBySomeone(damager);
		}
	}
}

void UOEntity::IHaveBeenKilledBySomeone(UOEntity * killer)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I have been killed by someone"));
}

