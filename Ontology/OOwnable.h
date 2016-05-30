// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
#include "OOwnable.generated.h"


class UOEntity;

UCLASS()
class OUTOFTHECAVE_01_API UOOwnable : public UItem
{
	GENERATED_BODY()

public:

	void ReceiveDamage(float damage);
	//void BePickedUp(UOEntity* entity);
	void BeStolen(UOEntity* buggler);

private:

	// Messages to owner for report generation
	void IHaveBeenDestroyedBySomeone();
	void IHaveBeenStolenBySomeone();
	void IHaveBeenGivenToSomeone();

	void DestroyOwnable();

	UOEntity* _owner;
	UOEntity* _attacker;
	UOEntity* _buggler;

	bool _meshDestroyable;
};
