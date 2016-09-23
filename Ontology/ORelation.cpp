// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"

int ORelation::LOW_APPRECIATION = 25;
int ORelation::REGULAR_APPRECIATION = 50;
int ORelation::HIGH_APPRECIATION = 75;

ORelation::ORelation()
{
}

ORelation::~ORelation()
{
}

ORelation::ORelation(UOEntity* entity, UOEntity* otherEntity, int appreciation, int respect, int fear) {
	_entity = entity;
	_otherEntity = otherEntity;
	_appreciation = ThresholdValue(appreciation);
	_respect = ThresholdValue(respect);
	_fear = ThresholdValue(fear);
}
ORelation::ORelation(UOEntity* entity, UOEntity* otherEntity) {
	_entity = entity;
	_otherEntity = otherEntity;
	_appreciation = ThresholdValue(entity->GetPersonality()->GetSocial());
	_respect = ThresholdValue(otherEntity->GetNotoriety());
	_fear = ThresholdValue(otherEntity->GetPersonality()->GetAggressiveness());
}

// GETTERS & SETTERS
void ORelation::SetAppreciation(int value) {
	_appreciation = ThresholdValue(value);
}
void ORelation::SetRespect(int value) {
	_respect = ThresholdValue(value);
}
void ORelation::SetFear(int value) {
	_fear = ThresholdValue(value);
}
UOEntity* ORelation::GetEntity() {
	return _entity;
}
UOEntity* ORelation::GetOtherEntity() {
	return _otherEntity;
}
int ORelation::GetAppreciation() {
	if (this != nullptr && this != NULL)
		return _appreciation;
	return 50;
}
int ORelation::GetRespect() {
	return _respect;
}
int ORelation::GetFear() {
	return _fear;
}


// CHANGERS
void ORelation::ChangeAppreciation(int value) {
	_appreciation = ThresholdValue(_appreciation + value);
}
void ORelation::ChangeRespect(int value) {
	_respect = ThresholdValue(_respect + value);
}
void ORelation::ChangeFear(int value) {

	_fear = ThresholdValue(_fear + value);
}


// THRESHOLD / CLAMP
int ORelation::ThresholdValue(int value) {
	if (value < _MIN_VALUE)
		return _MIN_VALUE;
	if (value > _MAX_VALUE)
		return _MAX_VALUE;
	return value;
}

bool ORelation::CheckConsistency() {
	return _entity != nullptr;
}
