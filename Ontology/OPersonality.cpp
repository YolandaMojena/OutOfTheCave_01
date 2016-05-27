// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OPersonality.h"

OPersonality::OPersonality()
{
}

OPersonality::~OPersonality()
{
}

OPersonality::OPersonality(int kind, int brave, int aggressive, int social)
{
	_kind = ThresholdValue(kind);
	_brave = ThresholdValue(brave);
	_aggressive = ThresholdValue(aggressive);
	_social = ThresholdValue(social);
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


// THRESHOLD / CLAMP
int OPersonality::ThresholdValue(int value) {
	if (value < _MIN_VALUE)
		return _MIN_VALUE;
	if (value > _MAX_VALUE)
		return _MAX_VALUE;
	return value;
}
