// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
#include "OOwnable.generated.h"


class UOEntity;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOOwnable : public UItem
{
	GENERATED_BODY()

public:

	void ReceiveDamage(float damage, UOEntity* damager);
	//void BePickedUp(UOEntity* entity);
	void BeStolen(UOEntity* buggler);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	bool meshDestroyable;

private:

	// Messages to owner for report generation
	void IHaveBeenDestroyedBySomeone(UOEntity* damager);
	void IHaveBeenStolenBySomeone();
	void IHaveBeenGivenToSomeone();

	void DestroyOwnable();

	UOEntity* _owner;
	UOEntity* _attacker;
	UOEntity* _buggler;

	bool _destroyed = false;
};
