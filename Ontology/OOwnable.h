// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
#include "OOwnable.generated.h"


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
	void BeStolen(UOEntity* buggler);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ownable)
	bool meshDestroyable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ownable)
		UOEntity* _owner;

private:

	// Messages to owner for report generation
	void IHaveBeenDestroyedBySomeone(UOEntity* damager);
	void IHaveBeenStolenBySomeone(UOEntity* buggler);
	void IHaveBeenGivenToSomeone();

	void DestroyOwnable();

	
	UOEntity* _attacker;
	UOEntity* _buggler;

	bool _destroyed = false;
};
