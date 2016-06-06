// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
#include "WorldSpawner.generated.h"

using namespace std;

class UOCivilian;

UCLASS()
class OUTOFTHECAVE_01_API AWorldSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//void SpawnCivilians();

private:

	/*TSubclassOf<class ACharacter> BP_Entity;
	TSubclassOf<class ACharacter> BP_Civilian_Goblin;
	TSubclassOf<class ACharacter> BP_Thing;
	TSubclassOf<class ACharacter> BP_Grabbable;
	TSubclassOf<class ACharacter> BP_Edification;

	vector<ACharacter*> _civilians;*/
};
