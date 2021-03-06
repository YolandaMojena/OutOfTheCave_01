// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OOwnership.h"

OOwnership::OOwnership()
{
}

OOwnership::~OOwnership()
{
}

OOwnership::OOwnership(UOEntity* owner, UOOwnable* item, int worth)
{
	_owner = owner;
	_item = item;
	_worth = worth;
}

UOEntity* OOwnership::GetOwner() {
	return _owner;
}

UOOwnable* OOwnership::GetOwnable() {
	return _item;
}

int OOwnership::GetWorth() {
	return _worth;
}

void OOwnership::SetWorth(int value) {
	_worth = ThresholdValue(value);
}

// THRESHOLD / CLAMP
int OOwnership::ThresholdValue(int value) {
	if (value < _MIN_VALUE)
		return _MIN_VALUE;
	if (value > _MAX_VALUE)
		return _MAX_VALUE;
	return value;
}