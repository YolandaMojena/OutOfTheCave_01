// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
#include <vector>
#include <algorithm>
#include "OOwnable.generated.h"

using namespace std;

class UOEntity;
class OOwnership;

UENUM(BlueprintType)
enum class Rarity : uint8 {
	common UMETA(DisplayName = "common"),
	uncommon UMETA(DisplayName = "uncommon"),
	rare UMETA(DisplayName = "rare"),
	mythic UMETA(DisplayName = "mythic")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOOwnable : public UItem
{
	GENERATED_BODY()

public:

	UOOwnable();
	~UOOwnable();

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//void BePickedUp(UOEntity* entity);
	void BeStolen(UOEntity* burglar);

	vector<UOEntity*> GetOwners();
	void AddOwner(UOEntity* e);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ownable)
		Rarity _rarity;
	Rarity GetRarity();
	int GetRarityAsInt();

private:

	// Messages to owner for report generation
	vector<UOEntity*> _owners;
	UOEntity* _buggler;

	void IHaveBeenStolenBySomeone(UOEntity* buggler);
	void IHaveBeenGivenToSomeone();
};
