// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/OEdification.h"
#include <vector>
#include "OResidence.generated.h"

class UOEntity;
class AVillage;

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UOResidence : public UOEdification
{
	GENERATED_BODY()
	
public:

	UOResidence();
	~UOResidence();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
		ERace race;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
		int32 numTenants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
		EJob job;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
	ERace herd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
	int32 herdSize;*/


	bool initialized = false;

	vector<UOEntity*> tentants;
	


private:
	void SpawnTenants();
	ACharacter* GetTentantCharacterFromRace();
	Graph* SetIdleFromJob();


	TSubclassOf<class ACharacter> BP_Civilian_Goblin;

};
