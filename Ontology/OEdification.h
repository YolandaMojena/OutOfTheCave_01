// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OCivilian.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "NarrativeGeneration/Report.h"
#include "Graph.h"
#include <vector>
#include <cstdlib>
#include "RebuildableEdification.h"
#include "OEdification.generated.h"

using namespace std;


class AVillage;


UENUM(BlueprintType)
enum class EdificationType : uint8 {
	house	UMETA(DisplayName = "House"), 
	field	UMETA(DisplayName = "Field"),
	ore		UMETA(DisplayName = "Ore"),
	tree	UMETA(Displayname = "Tree")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSettings)
		int32 villageID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSettings)
		int32 edificationID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSettings)
		EdificationType edificationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdificationSettings)
		float timeToRebuild = 2000.0f;

	void ReceiveDamage(float damage, UOEntity* damager, FVector collisionPos);
	bool GetIsDestroyed();

	void RebuildEdification(float addedValue);

protected:
	//virtual void UOOwnable::IHaveBeenDamagedBySomeone(UOEntity* damager) override;
	//virtual void UOOwnable::IHaveBeenDestroyedBySomeone(UOEntity* damager) override;

	AVillage* _village;
	void FindVillage();

private:

	void IHaveBeenDestroyedBySomeone(UOEntity* damager);
	void IHaveBeenDamagedBySomeone(UOEntity* damager);
	

	UOEntity* _attacker;
	bool _isDestroyed;
	void DestroyEdification(UOEntity* damager);

	float _accumulatedTime;

};
