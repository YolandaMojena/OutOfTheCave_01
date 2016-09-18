// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ontology/Item.h"
#include "Ontology/OOwnable.h"

class UOEntity;

/**
 * 
 */
class OUTOFTHECAVE_01_API OntologicFunctions
{
public:
	OntologicFunctions();
	~OntologicFunctions();

	UENUM(BlueprintType)
	enum AffordableUse : uint8{
		weapon		UMETA(DisplayName = "weapon"), 
		cultivator	UMETA(DisplayName = "cultivator"),
		mine		UMETA(DisplayName = "mine"),
		build		UMETA(DisplayName = "build")
	};

	int GetAffordance(AffordableUse au, UItem* i, UOEntity* u); // u = user

	int UseAsWeapon(UItem* i, UOEntity* u);
	//int Pierce(UItem* i, UOEntity* user);
	int UseAsCultivator(UItem* i, UOEntity* u);
	int UseForMining(UItem* i, UOEntity* u);
	int UseForBuilding(UItem* i, UOEntity* u);

	
private:
	
	float FE3(int value, int target, int permissionLeft, int permissionRight);
	float FE5(int value, int target, int permissionLeft, int permissionRight);
	float FEX(float x, int value, int target, int permissionLeft, int permissionRight);
	int FEVectorDist(FVector vectorValue, FVector vectorReference, int maxLength);
	int FEVectorDistAlongAxis(FVector vectorValue, FVector vectorReference, int maxLength, FVector axis);
	int FEVectorAngle(FVector vectorValue, FVector vectorReference);
	float VectorAngle(FVector a, FVector b);
	float Clamp01(float value);
	float Clamp100(float value);

	/*int HyperbolicGrowth(int value);
	int LinearGrowth(int x);
	int ExponentialGrowth(int x);
	int HyperbolicGrowth(int value, int magnitude);
	int LinearGrowth(int x, int m);
	int ExponentialGrowth(int x, int m);

	int HyperbolicDecay(int x, int m, int n);
	int HyperbolicDecay(int x, int m);
	int LinearDecay(int x, int m);
	int ExponentialDecay(int x, int m);
	int HyperbolicDecay(int );
	int LinearDecay(int x);
	int ExponentialDecay(int x);

	int HyperbolicEqualization(int x, int m);
	int LinearEqualization(int x, int m);
	int ExponentialEqualization(int x, int m);

	int ThresholdValue(int x, int t);
	int ThresholdValue(int x);
	int BotThresholdValue(int x);*/


};
