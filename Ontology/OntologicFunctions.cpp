// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include <math.h>
#include "OntologicFunctions.h"

OntologicFunctions::OntologicFunctions()
{
	_hands = NewObject<UOOwnable>();
	_hands->_name = FString(TEXT("hands"));
	_hands->_centerOfMass = FVector::ZeroVector;
	_hands->_edgeLength = 1;
	_hands->_edgeSharpness = 10;
	_hands->_funcDir = FVector(0, 0, 1);
	_hands->_funcPos = FVector::ZeroVector;
	_hands->_grabDir = FVector(0, 0, 1);
	_hands->_grabPos = FVector(0, 0, 0);
	_hands->_mass = 1;
	_hands->_maxLength = 10;
	_hands->_spikes = 5;
	_hands->_spiky = 1;
	_hands->_toughness = 30;
	_hands->_volume = 450;
}

OntologicFunctions::~OntologicFunctions()
{
}


int OntologicFunctions::GetAffordance(AffordableUse au, UItem* i) {
	switch (au) {
	case AffordableUse::cultivator:
		return UseAsCultivator(i);
	case AffordableUse::mine:
		return UseForMining(i);
	case AffordableUse::weapon:
		return UseAsWeapon(i);
	case AffordableUse::build:
		return UseForBuilding(i);
	default:
		return 0;
	}
}

int OntologicFunctions::UseAsWeapon(UItem* i) {
	int nfactors = 3;
	/*return ThresholdValue(ExponentialGrowth(i->GetToughness()))
		* (ThresholdValue(LinearGrowth(i->GetAngularMomentumTransmission())) * ThresholdValue(HyperbolicGrowth(i->GetMass(), 5)) / (pow(100,2-1))
			+ ThresholdValue(LinearGrowth(i->GetAngularInertia())) * ThresholdValue(HyperbolicDecay(i->GetMass(), 2))) / (pow(100, 2 - 1))
		* (ThresholdValue(LinearGrowth(i->GetElongation())) + (ThresholdValue(ExponentialGrowth(i->GetEdgeSharpness())) * ThresholdValue(HyperbolicGrowth(i->GetEdgeLength()))) / (pow(100, 2 - 1))
			+ ThresholdValue(ExponentialGrowth(i->GetSpiky())))
		/ (pow(100, nfactors - 1));*/

	int term1 = ThresholdValue(ExponentialGrowth(i->GetToughness()));
	int term2 = (ThresholdValue(LinearGrowth(i->GetAngularMomentumTransmission())) * ThresholdValue(HyperbolicGrowth(i->GetMass(), 5)) / (pow(100, 2 - 1))
		+ ThresholdValue(HyperbolicDecay(i->GetAngularInertia(), 10)) * ThresholdValue(HyperbolicDecay(i->GetMass(), 2))) / (pow(100, 2 - 1));
	int term3 = (ThresholdValue(LinearGrowth(i->GetElongation()))
		+ (ThresholdValue(ExponentialGrowth(i->GetEdgeSharpness())) * ThresholdValue(HyperbolicGrowth(i->GetEdgeLength()))) / (pow(100, 2 - 1))
		+ ThresholdValue(ExponentialGrowth(i->GetSpiky())));

	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   mass decay: ") + FString::SanitizeFloat(ThresholdValue(HyperbolicDecay(i->GetMass(), 2))));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 3: ") + FString::SanitizeFloat(term3));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 2: ") + FString::SanitizeFloat(term2));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 1: ") + FString::SanitizeFloat(term1));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("Evaluating: ") + i->GetName());

	int result = term1 * term2 * term3 / (pow(100, nfactors - 1));
	return result;

}
int OntologicFunctions::Pierce(UItem* i) {
	return i->GetToughness() * (i->GetEdgeSharpness() * i->GetEdgeLength() + i->GetSpiky() * i->GetSpikes());
}
int OntologicFunctions::UseAsCultivator(UItem* i){
	int nfactors = 4;
	int term1 = ThresholdValue(LinearGrowth(i->GetToughness(), 80));
	int term2 = ThresholdValue(LinearGrowth(i->GetElongation(), 1406));
	int term3 = (ThresholdValue(HyperbolicGrowth(i->GetEdgeSharpness(), 80)) * ExponentialEqualization(i->GetEdgeLength(), 20) / pow(100, (2 - 1)))
		+ ThresholdValue(ExponentialGrowth(i->GetSpikes(), 6));
	int term4 = ThresholdValue(ExponentialDecay(i->GetMass(), 3));
	
	/*GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 4: ") + FString::SanitizeFloat(term4));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 3: ") + FString::SanitizeFloat(term3));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 2: ") + FString::SanitizeFloat(term2));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 1: ") + FString::SanitizeFloat(term1));
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("Evaluating: ") + i->GetName());*/

	int result = term1 * term2 * term3 * term4 / (pow(100, nfactors - 1));

	return result;

	/*return ThresholdValue(LinearGrowth(i->GetToughness(), 80)) 
		* ThresholdValue(LinearGrowth(i->GetElongation(), 1406)) 
		* (ThresholdValue(HyperbolicGrowth(i->GetEdgeSharpness(), 80)) * ExponentialEqualization(i->GetEdgeLength(), 20) / pow(100,(2-1)))
			+ ThresholdValue(ExponentialGrowth(i->GetSpikes(), 6)) 
		* ThresholdValue(ExponentialDecay(i->GetMass(),3)) 
		/ (pow(100, nfactors-1));*/
}
int OntologicFunctions::UseForMining(UItem* i) {
	return i->GetToughness() * i->GetSpiky() * i->GetAngularMomentumTransmission() * i->GetMass();
}
int OntologicFunctions::UseForBuilding(UItem* i) {
	return i->GetToughness() * i->GetAngularMomentumTransmission() / i->GetMaxLength() / i->GetMass();
}


// GROWTH
int OntologicFunctions::HyperbolicGrowth(int x) {
	return HyperbolicGrowth(x, 100);
}
int OntologicFunctions::LinearGrowth(int x) {
	return LinearGrowth(x, 100);
}
int OntologicFunctions::ExponentialGrowth(int x) {
	return ExponentialGrowth(x, 100);
}
int OntologicFunctions::HyperbolicGrowth(int x, int m) {
	if(m!=0)
		return (int)((-1.0f / ((float)x / ((float)m/1.5f) + 1.0f) + 1.0f) * 0.6f * 277.777778f);
													//1.5f					0.6f
													//2.0f					0.54f
													//3.0f					0.48f
													//4.0f					0.45f
													//9.0f					0.9f
	
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("FUCK THIS HYPERBOLIC SHIT"));
	return -1;
}
int OntologicFunctions::LinearGrowth(int x, int m) {
	if (m != 0)
		return (int)((float)x / ((float)m / 100.0f));
	
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("FUCK THIS LINEAR SHIT"));
	return -1;
}
int OntologicFunctions::ExponentialGrowth(int x, int m) {
	return (int)(pow((float)LinearGrowth(x, m), 2.0f) / 100.0f);
}


// DECAY
int OntologicFunctions::HyperbolicDecay(int x, int m) {
	//return (int)((-1.0f / (((float)-x + 2.0f*(float)m) / ((float)m / 1.5f) + 1.0f) + 1.0f) * 0.6f * 100);
	return BotThresholdValue(HyperbolicGrowth(-x + 2 * m, m));
}
int OntologicFunctions::LinearDecay(int x, int m) {
	return BotThresholdValue(LinearGrowth(-x + 2 * m, m));
}
int OntologicFunctions::ExponentialDecay(int x, int m) {
	return (int)(pow((float)LinearDecay(x, m), 2.0f) / 100.0f);
}

int OntologicFunctions::HyperbolicDecay(int x) {
	return HyperbolicDecay(x, 100);
}
int OntologicFunctions::LinearDecay(int x) {
	return LinearDecay(x, 100);
}
int OntologicFunctions::ExponentialDecay(int x) {
	return ExponentialDecay(x, 100);
}


// EQUALIZATION
int OntologicFunctions::HyperbolicEqualization(int x, int m) {
	if (x < m)
		return HyperbolicGrowth(x, m);
	else
		return HyperbolicDecay(x, m);
}
int OntologicFunctions::LinearEqualization(int x, int m) {
	if (x < m)
		return LinearGrowth(x, m);
	else
		return LinearDecay(x, m);
}
int OntologicFunctions::ExponentialEqualization(int x, int m) {
	if (x < m)
		return ExponentialGrowth(x, m);
	else
		return ExponentialDecay(x, m);
}


// THRESHOLD
int OntologicFunctions::ThresholdValue(int x, int t) {
	return (x > t ? t : x);
}
int OntologicFunctions::ThresholdValue(int x) {
	return ThresholdValue(x, 100);
}
int OntologicFunctions::BotThresholdValue(int x) {
	return (x > 0 ? x : 0);
}


UOOwnable* OntologicFunctions::GetHands() {
	return _hands;
}