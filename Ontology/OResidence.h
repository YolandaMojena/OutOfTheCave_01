// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/OEdification.h"
#include <vector>
#include <cstdlib>
#include <time.h> 
#include "OResidence.generated.h"

class UOEntity;
class UOCivilian;
class AVillage;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOResidence : public UOEdification
{
	GENERATED_BODY()
	
public:

	UOResidence();
	~UOResidence();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ResidenceSettings)
		ERace race;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ResidenceSettings)
		int32 numTenants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ResidenceSettings)
		EJob job;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ResidenceSettings)
	ERace herd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ResidenceSettings)
	int32 herdSize;*/


	bool initialized = false;

	vector<UOEntity*> tentants;
	
	void IWantToGetInside(UOEntity* e);
	void IWantToGetOut(UOEntity* e);


private:
	void SpawnTenants();
	ACharacter* GetTentantCharacterFromRace();
	Graph* GenerateIdleFromJob();

	FVector UOResidence::RandomDisplacementVector(int radius);

	FString AssignFemaleHumanName();
	FString AssignMaleHumanName();
	FString AssignGoblinName();

	TSubclassOf<class ACharacter> BP_Civilian_Goblin;
	TSubclassOf<class ACharacter> BP_Civilian_Human_Male;
	TSubclassOf<class ACharacter> BP_Civilian_Human_Female;

	vector<UOEntity*> _inside;

	TArray<FString> _goblinNames;
	TArray<FString> _maleHumanNames;
	TArray<FString> _femaleHumanNames;

	const FString _savedPath = FPaths::GameDir() + "SavedFiles/"/*"C:\\Users\\Yolanda\\Desktop\\SavedFiles\\"*/;
	const FString _goblinPath = "OOTC_goblinNames.txt";
	const FString _femaleHumanPath = "OOTC_femaleNames.txt";
	const FString _maleHumanPath = "OOTC_maleNames.txt";
};
