// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Relation.h"
#include "Thing.h"
#include "Entity.h"
#include <vector>
#include "GameWorldManager.generated.h"


using namespace std;

UCLASS()
class OUTOFTHECAVE_01_API AGameWorldManager : public AActor
{
	GENERATED_BODY()

public:

	AGameWorldManager(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void InitRelations();

	vector<AEntity*> worldEntities;
	vector<AThing*> worldThings;

	vector<Relation> worldRelations;
};
