// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OCivilian.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Graph.h"
#include <vector>
#include <cstdlib>
#include "OEdification.generated.h"

using namespace std;


class AVillage;

/**
 * 
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOEdification : public UOOwnable
{
	GENERATED_BODY()
	
public:

	UOEdification();
	~UOEdification();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
		int32 villageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
		int32 edificationID;


protected:
	//virtual void UOOwnable::IHaveBeenDamagedBySomeone(UOEntity* damager) override;
	//virtual void UOOwnable::IHaveBeenDestroyedBySomeone(UOEntity* damager) override;

	AVillage* _village;

private:
	void FindVillage();
};
