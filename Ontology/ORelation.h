// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/OEntity.h"
/**
 * 
 */
class OUTOFTHECAVE_01_API ORelation
{
public:
	ORelation(UOEntity* entity, UOEntity* other, int appreciation, int respect, int fear);
	ORelation();
	~ORelation();

	UOEntity* GetEntity();
	UOEntity* GetOtherEntity();

	void ChangeAppreciation(int value);
	void ChangeRespect(int value);
	void ChangeFear(int value);

	void SetAppreciation(int value);
	void SetRespect(int value);
	void SetFear(int value);
	
	int GetAppreciation();
	int GetRespect();
	int GetFear();

	bool CheckConsistency();


private:
	UOEntity* _entity;
	UOEntity* _otherEntity;
	int _appreciation;
	int _respect;
	int _fear;

	const int _MIN_VALUE = 0;
	const int _MAX_VALUE = 100;

	int ThresholdValue(int value);
};
