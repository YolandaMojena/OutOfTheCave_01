// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ontology/OEdification.h"
#include <vector>
#include "NarrativeGeneration/Utilities.h"
#include "Village.generated.h"


using namespace std;
/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API AVillage : public AActor
{
	GENERATED_BODY()
public:
	AVillage();
	~AVillage();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
		int32 villageID;

	vector<UOEdification*> edifications;

	FString AssignFemaleHumanName();
	FString AssignMaleHumanName();
	FString AssignGoblinName();

private:

	TArray<FString> _goblinNames;
	TArray<FString> _maleHumanNames;
	TArray<FString> _femaleHumanNames;

};
