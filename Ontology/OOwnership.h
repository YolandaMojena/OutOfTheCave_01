// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/OOwnable.h"
#include "Ontology/OEntity.h"
/**
 * 
 */
class OUTOFTHECAVE_01_API OOwnership
{
public:
	OOwnership(UOEntity* owner, UOOwnable* item, int worth);
	OOwnership();
	~OOwnership();

	UOEntity* GetOwner();
	UOOwnable* GetItem();
	int GetWorth();

	void SetWorth(int value);

private:

	UOOwnable* _item;
	UOEntity* _owner;
	int _worth;

	const int _MIN_VALUE = 0;
	const int _MAX_VALUE = 100;

	int ThresholdValue(int value);
};
