// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ontology/OEdification.h"
#include <vector>
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
		int32 villageID;

	vector<UOEdification*> edifications;
};
