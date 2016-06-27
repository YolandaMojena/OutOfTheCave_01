// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/OEdification.h"
#include "OTree.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOTree : public UOEdification
{
	GENERATED_BODY()

public:

	UOTree();
	~UOTree();

	TSubclassOf<class AActor> BP_Tree;

	void SpawnLeaflessTree();
};
