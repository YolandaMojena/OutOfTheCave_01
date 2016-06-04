// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
#include <vector>
#include <algorithm>
#include "OOwnable.generated.h"

using namespace std;

class UOEntity;
class OOwnership;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOOwnable : public UItem
{
	GENERATED_BODY()

public:

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float damage, UOEntity* damager);
	//void BePickedUp(UOEntity* entity);
	void BeStolen(UOEntity* burglar);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ownable)
	bool meshDestroyable;

	vector<UOEntity*> _owners;

protected:
	void IHaveBeenDamagedBySomeone(UOEntity* damager);
	void IHaveBeenDestroyedBySomeone(UOEntity* damager);
	void IHaveBeenStolenBySomeone(UOEntity* buggler);
	void IHaveBeenGivenToSomeone();

private:

	// Messages to owner for report generation
	

	void DestroyOwnable();

	
	UOEntity* _attacker;
	UOEntity* _buggler;

	bool _destroyed = false;
};
