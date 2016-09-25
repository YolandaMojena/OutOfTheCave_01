// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include <math.h>
#include "Ontology/OEntity.h"
#include "OntologicFunctions.h"

OntologicFunctions::OntologicFunctions()
{
}

OntologicFunctions::~OntologicFunctions()
{
}


int OntologicFunctions::GetAffordance(AffordableUse au, UItem* i, UOEntity* u) {
	switch (au) {
	case AffordableUse::cultivator:
		return UseAsCultivator(i, u);
	case AffordableUse::mine:
		return UseForMining(i, u);
	case AffordableUse::weapon:
		return UseAsWeapon(i, u);
	case AffordableUse::build:
		return UseForBuilding(i, u);
	default:
		return 0;
	}
}


int OntologicFunctions::UseAsWeapon(UItem* i, UOEntity* u) {
	float nTerms = 4.f;

	float term1 = FE3(i->GetToughness(), 3, 3, 0);
	float term2 = FE3(i->GetElongation(), 3, 2, 0);
	float term3 = FE5(i->GetVolume(), u->GetVolume() - 1, 1, 1);
	float term4 = Clamp01(
		sqrt(pow(((FE5(i->GetMass(), 2, 0, 2) + FE3(i->GetEdgeSharpness(), 3, 2, 0) + FE3(i->GetEdgeLength(), 3, 2, 0))/3.f),2)	//Sword Mode
			+ pow(((FE5(i->GetMass(), 3, 2, 0) + FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength()), 3, 2, 0) + FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength()), 1, 0, 3)) / 2.f), 2.f))); // Warhammer Mode;
		//+ pow(((FE5(i->GetMass(), 3, 1, 0) + FE3(i->GetEdgeSharpness(), 3, 2, 0) + FE3(i->GetEdgeLength(), 2, 3, 2) + FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength()), 3, 2, 0) + FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength()), 1, 0, 3))/5.f),2.f)	// Axe Mode
		//+ pow(((FE5(i->GetMass(), 3, 2, 0) + FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength()), 3, 2, 0) + FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength()), 1, 0, 3))/3.f),2.f))); // Warhammer Mode;
	float bonusTerm = 0.25 * FE3(i->GetToughness(), 3,3,0) * (FE3(i->GetSpiky(), 3, 3, 0) + FE3(i->GetNbrSpikes(), 3, 2, 0))/2.f; // Morning Star bonus;

	float result = (term1 + term2 + term3 + term4) / nTerms + bonusTerm;

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, i->GetItemName() + TEXT(" as weapon: ") + FString::SanitizeFloat(result));

	return Clamp100(result*100);

	/*return Clamp100(100 * FE3(i->GetToughness(),3,1,0) * FE3(i->GetElongation(),3,1,0) * FE5(i->GetVolume(),u->GetVolume()-1,1,1) * Clamp01(
		FE5(i->GetMass(),2,0,2) * FE3(i->GetEdgeSharpness(),3,2,0) * FE3(i->GetEdgeLength(),3,2,0)	//Sword Mode
		+ FE5(i->GetMass(),3,1,0) * FE3(i->GetEdgeSharpness(),3,2,0) * FE3(i->GetEdgeLength(),2,1,2) * FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength()),3,2,0) * FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength()),1,0,3)	// Axe Mode
		+ FE5(i->GetMass(),3,2,0) * FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength()),3,2,0) * FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength()),1,0,3)) // Warhammer Mode
		+ 25 * FE3(i->GetSpiky(),3,3,0) * FE3(i->GetNbrSpikes(),3,1,0)); // Morning Star bonus*/
}

int OntologicFunctions::UseAsCultivator(UItem* i, UOEntity* u) {
	float nTerms = 5;

	float term1 = FE3(i->GetToughness(), 2, 2, 0);
	float term2 = FE3(i->GetElongation(), 3, 1, 0);
	float term3 = FE5(i->GetVolume(), u->GetVolume() - 1, 2, 2);
	float term4 = FE5(i->GetMass(), u->GetMass() - 2, 0, 1);
	float term5 = Clamp01(
		(FE3(i->GetEdgeSharpness(), 2, 3, 0) + FE3(i->GetEdgeLength(), 1, 0, 1) + FE3(FEVectorAngle(i->R_GetFuncPlane(), i->R_GetGrabDir()), 1, 0, 3)) / 3.f
		+ (FE3(i->GetSpiky(), 2, 3, 0) + FE3(i->GetNbrSpikes(), 3, 3, 0))/2.f);

	float result = (term1 + term2 + term3 + term4 + term5)/nTerms;

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, i->GetItemName() + TEXT(" for cultivating: ") + FString::SanitizeFloat(result));
	
	return Clamp100(result*100.f);
	/*return Clamp100(100 * FE3(i->GetToughness(),2,3,0) * FE3(i->GetElongation(),3,1,0) * FE5(i->GetVolume(), u->GetVolume() - 1,2,2) * FE5(i->GetMass(),1,0,2) * Clamp01(
		FE3(i->GetEdgeSharpness(),2,3,0) * FE3(i->GetEdgeLength(),1,0,1) * FE3(FEVectorAngle(i->R_GetFuncPlane(),i->R_GetGrabDir()),1,0,3)
		+ FE3(i->GetSpiky(),2,3,0) * FE3(i->GetNbrSpikes(),3,3,0)));*/
}
int OntologicFunctions::UseForMining(UItem* i, UOEntity* u) {
	float nTerms = 8;

	float term1 = FE3(i->GetToughness(), 3, 3, 0);
	float term2 = FE3(i->GetElongation(), 2, 3, 1);
	float term3 = FE5(i->GetVolume(), u->GetVolume() - 1, 2, 1);
	float term4 = FE5(i->GetMass(), 2, 2, 0);
	float term5 = FE3(i->GetSpiky(), 3, 3, 0);
	float term6 = FE3(i->GetNbrSpikes(), 1, 0, 1);
	float term7 = FE3(FEVectorDistAlongAxis(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength(), i->R_GetGrabDir()), 3, 2, 0);
	float term8 = FE3(FEVectorDistAlongAxis(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength(), i->R_GetGrabDir()), 1, 0, 3);

	float result = (term1 + term2 + term3 + term4 + term5 + term6 + term7 + term8) / nTerms;

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, i->GetItemName() + TEXT(" for mining: ") + FString::SanitizeFloat(result));

	return Clamp100(result*100.f);
	/*return Clamp100(100 * FE3(i->GetToughness(),3,3,0) * FE3(i->GetElongation(),3,2,0) * FE5(i->GetVolume(),u->GetVolume()-1,2,2) * FE5(i->GetMass(),2,2,0) *  FE3(i->GetSpiky(), 3, 3, 0) * FE3(i->GetNbrSpikes(), 1, 0, 1)
		* FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength()), 3, 2, 0) * FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength()), 1, 0, 3));*/
}
int OntologicFunctions::UseForBuilding(UItem* i, UOEntity* u) {
	float nTerms = 5;

	float term1 = FE3(i->GetToughness(), 2, 3, 0);
	float term2 = FE3(i->GetElongation(), 3, 1, 0);
	float term3 = FE5(i->GetVolume(), u->GetVolume() - 2, 2, 2);
	float term4 = FE3(FEVectorDistAlongAxis(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength(), i->R_GetGrabDir()), 3, 2, 0);
	float term5 = FE3(FEVectorDistAlongAxis(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength(), i->R_GetGrabDir()), 1, 0, 3);

	float result = (term1 + term2 + term3 + term4 + term5) / nTerms;

	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, i->GetItemName() + TEXT(" for building: ") + FString::SanitizeFloat(result));

	return Clamp100(result*100.f);
	/*return Clamp100(100 * FE3(i->GetToughness(),2,3,0) * FE3(i->GetElongation(), 3,1,0) * FE5(i->GetVolume(),u->GetVolume()-2,2,2)
		* FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetGrabPos(), i->R_GetMaxLength()), 3, 2, 0) * FE3(FEVectorDist(i->R_GetFuncPos(), i->R_GetCenterOfMass(), i->R_GetMaxLength()), 1, 0, 3));*/
}






float OntologicFunctions::FE3(int value, int target, int permissionLeft, int permissionRight) {
	return FEX(3.f, value, target, permissionLeft, permissionRight);
}
float OntologicFunctions::FE5(int value, int target, int permissionLeft, int permissionRight) {
	return FEX(5.f, value, target, permissionLeft, permissionRight);
}
float OntologicFunctions::FEX(float x, int value, int target, int permissionLeft, int permissionRight) {

	if (value < 1 || value > x) {
		//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, FString::SanitizeFloat(value) + TEXT(" out of bounds!"));
		return 0.f;
	}

	if (value == target)
		return 1.f;

	int permission = value < target ? permissionLeft : permissionRight;

	switch (permission) {
	case 0:
		return 1.f;
	case 1:
		return (x - abs(target - value)) / x;
	case 2:
		return abs(target - value) == 1 ? 0.5f : 0.f;
	case 3:
		return 0.f;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("Wrong Permission!"));
		return -1.f;
	}
}
int OntologicFunctions::FEVectorDist(FVector vectorValue, FVector vectorReference, int maxLength) {
	float distance = FVector::Dist(vectorValue, vectorReference);
	distance /= maxLength;
	if (distance <= 10)
		return 1;
	else if (distance <= 75)
		return 2;
	else
		return 3;
}
int OntologicFunctions::FEVectorDistAlongAxis(FVector vectorValue, FVector vectorReference, int maxLength, FVector axis) {
	//vectorValue *= cos(VectorAngle(vectorValue, axis));
	//vectorReference *= cos(VectorAngle(vectorReference, axis));
	vectorValue = vectorReference + (vectorValue - vectorReference) * cos(VectorAngle((vectorValue - vectorReference), axis));
	return FEVectorDist(vectorValue, vectorReference, maxLength);
}

int OntologicFunctions::FEVectorAngle(FVector vectorValue, FVector vectorReference) {
	float angle = VectorAngle(vectorValue, vectorReference);

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, "angle: " + FString::SanitizeFloat(angle));

	
	if (angle > PI / 2.f) {
		angle = PI - angle;
		//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, "angle adjusted: " + FString::SanitizeFloat(angle));
	}
		
	if (angle < PI / 8.f)
		return 1;
	else if (angle < PI - PI / 8.f)
		return 2;
	else
		return 3;
}

float OntologicFunctions::VectorAngle(FVector a, FVector b) {
	a = a.GetSafeNormal();
	b = b.GetSafeNormal();
	float dot = FVector::DotProduct(a, b);
	return acos(dot);
}


float OntologicFunctions::Clamp01(float value) {
	if (value < 0)
		return 0.f;
	if (value > 1)
		return 1.f;
	return value;
}

float OntologicFunctions::Clamp100(float value) {
	if (value < 0)
		return 0.f;
	if (value > 100)
		return 100.f;
	return value;
}









// D E P R E C A T E D

/*int OntologicFunctions::UseAsWeapon(UItem* i) {
	int nfactors = 4;
	//return ThresholdValue(ExponentialGrowth(i->GetToughness()))
	//	* (ThresholdValue(LinearGrowth(i->GetAngularMomentumTransmission())) * ThresholdValue(HyperbolicGrowth(i->GetMass(), 5)) / (pow(100,2-1))
	//		+ ThresholdValue(LinearGrowth(i->GetAngularInertia())) * ThresholdValue(HyperbolicDecay(i->GetMass(), 2))) / (pow(100, 2 - 1))
	//	* (ThresholdValue(LinearGrowth(i->GetElongation())) + (ThresholdValue(ExponentialGrowth(i->GetEdgeSharpness())) * ThresholdValue(HyperbolicGrowth(i->GetEdgeLength()))) / (pow(100, 2 - 1))
	//		+ ThresholdValue(ExponentialGrowth(i->GetSpiky())))
	//	/ (pow(100, nfactors - 1));

	int term1 = ThresholdValue(ExponentialGrowth(i->GetToughness()));
	int term2 = (ThresholdValue(LinearGrowth(i->GetAngularMomentumTransmission())) * ThresholdValue(HyperbolicGrowth(i->GetMass(), 7)) / (pow(100, 2 - 1))
		+ ThresholdValue(HyperbolicDecay(i->GetAngularInertia(), 10)) * ThresholdValue(HyperbolicDecay(i->GetMass(), 2)) / (pow(100, 2 - 1)));
	int term3 = ThresholdValue(LinearGrowth(i->GetElongation()));
	int term4 = (ThresholdValue(ExponentialGrowth(i->GetEdgeSharpness())) * ThresholdValue(HyperbolicGrowth(i->GetEdgeLength()))) / (pow(100, 2 - 1))
		+ ThresholdValue(ExponentialGrowth(i->GetSpiky()));

	int result = ThresholdValue(term1) * ThresholdValue(term2) * ThresholdValue(term3) * ThresholdValue(term4) / (pow(100, nfactors - 1));

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   score: ") + FString::SanitizeFloat(result));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 4: ") + FString::SanitizeFloat(term4));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 3: ") + FString::SanitizeFloat(term3));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 2: ") + FString::SanitizeFloat(term2));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 1: ") + FString::SanitizeFloat(term1));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("Evaluating: ") + i->GetName());

	return result;

}
int OntologicFunctions::Pierce(UItem* i) {
	return i->GetToughness() * (i->GetEdgeSharpness() * i->GetEdgeLength() + i->GetSpiky() * i->GetSpikes());
}
int OntologicFunctions::UseAsCultivator(UItem* i){
	int nfactors = 4;
	int term1 = ThresholdValue(LinearGrowth(i->GetToughness(), 80));
	int term2 = ThresholdValue(LinearGrowth(i->GetElongation(), 1200));
	int term3 = (ThresholdValue(HyperbolicGrowth(i->GetEdgeSharpness(), 80)) * ExponentialEqualization(i->GetEdgeLength(), 20) / pow(100, (2 - 1)))
		+ ThresholdValue(ExponentialGrowth(i->GetSpikes(), 6));
	int term4 = ThresholdValue(ExponentialDecay(i->GetMass(), 3));
	
	int result = term1 * term2 * term3 * term4 / (pow(100, nfactors - 1));

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   score: ") + FString::SanitizeFloat(result));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 4: ") + FString::SanitizeFloat(term4));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 3: ") + FString::SanitizeFloat(term3));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 2: ") + FString::SanitizeFloat(term2));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("   term 1: ") + FString::SanitizeFloat(term1));
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("Evaluating: ") + i->GetName());

	return result;

	//return ThresholdValue(LinearGrowth(i->GetToughness(), 80)) 
	//	* ThresholdValue(LinearGrowth(i->GetElongation(), 1406)) 
	//	* (ThresholdValue(HyperbolicGrowth(i->GetEdgeSharpness(), 80)) * ExponentialEqualization(i->GetEdgeLength(), 20) / pow(100,(2-1)))
	//		+ ThresholdValue(ExponentialGrowth(i->GetSpikes(), 6)) 
	//	* ThresholdValue(ExponentialDecay(i->GetMass(),3)) 
	//	/ (pow(100, nfactors-1));
}
int OntologicFunctions::UseForMining(UItem* i) {
	int nfactors = 3;

	int term1 = ThresholdValue(ExponentialGrowth(i->GetToughness(),90));
	int term2 = ThresholdValue(ExponentialGrowth(i->GetSpiky(), 90));
	int term3 = ThresholdValue(ExponentialGrowth(i->GetAngularMomentumTransmission()));
	//int term4 = ThresholdValue(HyperbolicGrowth(i->GetMass(),5));
	int result = term1 * term2 * term3 / (pow(100, nfactors - 1));

	return result;
}
int OntologicFunctions::UseForBuilding(UItem* i) {
	int nfactors = 4;

	int term1 = ThresholdValue(ExponentialGrowth(i->GetToughness(), 90));
	int term2 = ThresholdValue(LinearGrowth(i->GetAngularMomentumTransmission()));
	int term3 = ThresholdValue(LinearEqualization(i->GetMaxLength(), 55));
	int term4 = ThresholdValue(ExponentialDecay(i->GetMass(), 1));
	
	int result = term1 * term2 * term3 * term4 / (pow(100, nfactors - 1));

	return result;

	//return i->GetToughness() * i->GetAngularMomentumTransmission() / i->GetMaxLength() / i->GetMass();
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
int OntologicFunctions::HyperbolicDecay(int x, int m, int n) {
	if (m < n)
		return BotThresholdValue((int)((-1.0f / (((float)-x + (float)n) / (((float)n- (float)m) / 1.5f) + 1.0f) + 1.0f) * 0.6f * 277.777778f));

	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("FUCK THIS HYPERBOLIC DECAY SHIT"));
	return -1;
}
int OntologicFunctions::HyperbolicDecay(int x, int m) {
	//return (int)((-1.0f / (((float)-x + 2.0f*(float)m) / ((float)m / 1.5f) + 1.0f) + 1.0f) * 0.6f * 100);
	//return BotThresholdValue(HyperbolicGrowth(-x + 2 * m, m));
	//if(m < 100.f)
	//	return BotThresholdValue((int)((-1.0f / (((float)-x + 100.f)/ ((100.f - m) / 1.5f) + 1.0f) + 1.0f) * 0.6f * 277.777778f));
	//
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("FUCK THIS HYPERBOLIC DECAY SHIT"));
	//return -1;
	return HyperbolicDecay(x, m, 2 * m);
}
int OntologicFunctions::LinearDecay(int x, int m) {
	return BotThresholdValue(LinearGrowth(-x + 2 * m, m));
}
int OntologicFunctions::ExponentialDecay(int x, int m) {
	return BotThresholdValue((int)(pow((float)LinearDecay(x, m), 2.0f) / 100.0f));
}

int OntologicFunctions::HyperbolicDecay(int x) {
	return HyperbolicDecay(x, 0, 100);
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
}*/
