// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
/**
 * 
 */
class OUTOFTHECAVE_01_API OOwnership
{
public:
	OOwnership(UItem* item, int worth);
	OOwnership();
	~OOwnership();

	UItem* GetItem();
	int GetWorth();

	void SetWorth(int value);

private:
	UItem* _item;
	int _worth;

	const int _MIN_VALUE = 0;
	const int _MAX_VALUE = 100;

	int ThresholdValue(int value);
};
