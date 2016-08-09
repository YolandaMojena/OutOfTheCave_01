// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ontology/Item.h"
#include "Ontology/OOwnable.h"
#include "OwnableSpawner.generated.h"

UCLASS()
class OUTOFTHECAVE_01_API AOwnableSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOwnableSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	AActor* SpawnOwnable(UOOwnable* o, UItem* spawner);

private:
	TSubclassOf<class AActor> BP_Hammer;
	TSubclassOf<class AActor> BP_Hoe;
	TSubclassOf<class AActor> BP_Pickaxe;
	TSubclassOf<class AActor> BP_Rake;
	TSubclassOf<class AActor> BP_Sword;
	TSubclassOf<class AActor> BP_LeaflessTree;

	FVector RandomDisplacementVector(int radius);
};
