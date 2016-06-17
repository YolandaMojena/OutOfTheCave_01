// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ontology/Item.h"
#include "Ontology/OOwnable.h"

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

	int GetAffordance(AffordableUse au, UItem* i);

	int UseAsWeapon(UItem* i);
	int Pierce(UItem* i);
	int UseAsCultivator(UItem* i);
	int UseForMining(UItem* i);
	int UseForBuilding(UItem* i);

	UOOwnable* GetHands();


	/*struct PriorityCandidates {
		PriorityCandidates(AffordableUse au) {
			_au = au;
		}
		PriorityCandidates() {}
		~PriorityCandidates() {}
		
		bool operator()(UOOwnable& A, UOOwnable& B) const {
			switch (_au) {
			case AffordableUse::build:
				return true;
			case AffordableUse::cultivator:
				return true;
			case AffordableUse::mine:
				return true;
			case AffordableUse::weapon:
				return true;
			default:
				return true;
			}
			
		}

	private:
		AffordableUse _au;
	};*/

	


private:
	int ReduceWeight(int value);
	int AmplifyWeight(int value);
	
	int HyperbolicGrowth(int value);
	int LinearGrowth(int x);
	int ExponentialGrowth(int x);
	int HyperbolicGrowth(int value, int magnitude);
	int LinearGrowth(int x, int m);
	int ExponentialGrowth(int x, int m);

	int HyperbolicDecay(int x, int m);
	int LinearDecay(int x, int m);
	int ExponentialDecay(int x, int m);

	int HyperbolicEqualization(int x, int m);
	int LinearEqualization(int x, int m);
	int ExponentialEqualization(int x, int m);

	int ThresholdValue(int x, int t);
	int ThresholdValue(int x);
	int BotThresholdValue(int x);

	UOOwnable _hands;
};
