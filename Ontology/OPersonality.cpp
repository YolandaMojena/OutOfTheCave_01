// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OPersonality.h"

OPersonality::OPersonality()
{
	_kind = rand() % (_MAX_VALUE + 1);
	_brave = rand() % (_MAX_VALUE + 1);
	_aggressive = rand() % (_MAX_VALUE + 1);
	_social = rand() % (_MAX_VALUE + 1);
	_materialist = rand() % (_MAX_VALUE + 1);
}

OPersonality::~OPersonality()
{
}

OPersonality::OPersonality(int kind, int brave, int aggressive, int social, int materialist)
{
	_kind = ThresholdValue(kind);
	_brave = ThresholdValue(brave);
	_aggressive = ThresholdValue(aggressive);
	_social = ThresholdValue(social);
	_materialist = ThresholdValue(materialist);
}

int OPersonality::GetKindness() {
	return _kind;
}
int OPersonality::GetBraveness() {
	return _brave;
}
int OPersonality::GetAggressiveness() {
	return _aggressive;
}
int OPersonality::GetSocial() {
	return _social;
}
int OPersonality::GetMaterialist() {
	return _materialist;
}

int OPersonality::GetMinValue() {
	return _MIN_VALUE;
}
int OPersonality::GetMaxValue() {
	return _MAX_VALUE;
}


// THRESHOLD / CLAMP
int OPersonality::ThresholdValue(int value) {
	if (value < _MIN_VALUE)
		return _MIN_VALUE;
	if (value > _MAX_VALUE)
		return _MAX_VALUE;
	return value;
}
