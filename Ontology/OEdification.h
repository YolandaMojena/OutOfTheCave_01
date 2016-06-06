// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OCivilian.h"
#include <vector>
#include <cstdlib>
#include "OEdification.generated.h"

using namespace std;

class UOEntity;

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UOEdification : public UOOwnable
{
	GENERATED_BODY()
	
public:

	UOEdification();
	~UOEdification();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int villageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int raceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int numTenants;

	void SpawnTenants();

protected:
	//virtual void UOOwnable::IHaveBeenDamagedBySomeone(UOEntity* damager) override;
	//virtual void UOOwnable::IHaveBeenDestroyedBySomeone(UOEntity* damager) override;

private:

	TSubclassOf<class ACharacter> BP_Civilian_Goblin;

	vector<UOEntity*> _tentants;

	
	
};
