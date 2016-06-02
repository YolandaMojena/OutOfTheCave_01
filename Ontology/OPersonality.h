// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class OUTOFTHECAVE_01_API OPersonality
{
public:
	OPersonality(int kind, int brave, int aggressive, int social, int materialist);
	OPersonality();
	~OPersonality();

	int GetKindness();
	int GetBraveness();
	int GetAggressiveness();
	int GetSocial();
	int GetMaterialist();

	int GetMinValue();
	int GetMaxValue();

	int ThresholdValue(int value);

private:
	const int _MIN_VALUE = 0;
	const int _MAX_VALUE = 100;

	int _kind;
	int _brave;
	int _aggressive;
	int _social;
	int _materialist;
};
