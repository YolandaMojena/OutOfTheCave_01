// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ontology/Item.h"
#include <unordered_map>
#include <vector>
#include "Arquetypes.generated.h"

using namespace std;

UCLASS()
class OUTOFTHECAVE_01_API AArquetypes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArquetypes();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	unordered_map<string, vector<UItem*>> arquetypeDictionary;
	

};
