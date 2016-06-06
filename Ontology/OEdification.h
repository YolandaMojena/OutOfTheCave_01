// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OCivilian.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include <vector>
#include <cstdlib>
#include "OEdification.generated.h"

using namespace std;

class UOEntity;
class AVillage;

/**
 * 
 */


UENUM(BlueprintType)
enum class ERace : uint8 {
	R_Human		UMETA(DisplayName = "Human"),
	R_Goblin	UMETA(DisplayName = "Goblin"),
	R_Mixt		UMETA(DisplayName = "Mixt")
};

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
		ERace race;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSpawner)
		int32 numTenants;

	bool initialized = false;
	
	vector<UOEntity*> tentants;


protected:
	//virtual void UOOwnable::IHaveBeenDamagedBySomeone(UOEntity* damager) override;
	//virtual void UOOwnable::IHaveBeenDestroyedBySomeone(UOEntity* damager) override;

private:

	TSubclassOf<class ACharacter> BP_Civilian_Goblin;

	

	AVillage* _village;

	void FindVillage();
	void SpawnTenants();
};
