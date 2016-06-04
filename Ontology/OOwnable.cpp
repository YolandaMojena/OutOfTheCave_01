// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/OEntity.h"
#include "Ontology/OCivilian.h"
#include "Ontology/OOwnership.h"
#include "Ontology/ORelation.h"
#include "NarrativeGeneration/Report.h"
#include "Ontology/OOwnable.h"

void UOOwnable::BeginPlay() {

	Super::BeginPlay();

	//At the moment, last found entity is owner
	/*for (TObjectIterator<UOCivilian> Itr; Itr; ++Itr){
		_owners.push_back(*Itr);
		Itr->AddPossession(new OOwnership(*Itr, this, Itr->GetPersonality()->GetMaterialist()));
	}*/
}

void UOOwnable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!_canBeDamaged) {
		_hurtCooldown += DeltaTime;

		if (_hurtCooldown >= _HURT_COOLDOWN) {
			_hurtCooldown = 0;
			_canBeDamaged = true;
		}
	}
}

void UOOwnable::ReceiveDamage(float damage, UOEntity* damager) {

	if (!_destroyed && _canBeDamaged) {

		_integrity -= damage;
		_attacker = damager;
		_canBeDamaged = false;

		if (_integrity <= 0) {
			_destroyed = true;
			DestroyOwnable();
			IHaveBeenDestroyedBySomeone(damager);
		}
	}
}

// AT THE MOMENT
// NOTIFIES OWNER
// ELIMINATES POSSESSIN
// SETS OR MODIFIES RELATIONSHIP WITH DAMAGER
// GENERATES REPORT

void UOOwnable::IHaveBeenDestroyedBySomeone(UOEntity* damager)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("I have been destroyed by " + damager->GetOwner()->GetName()));

	/*for (int i = 0; i < _owners.size(); i++) {

		// Report hate towards damager
		OOwnership* ownership = _owners[i]->GetOwnershipWith(this);
		ORelation* oldRelation = _owners[i]->GetRelationWith(damager);

		if (ownership && oldRelation) {

			oldRelation->SetAppreciation(oldRelation->GetAppreciation() - ownership->GetWorth());
			_owners[i]->SendReport(new Report(_owners[i]->GetRelationWith(damager), {BasePlot::TypeOfPlot::aggressive}, this));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inexisting relationship or ownership"));

		// Delete from owners possessions and report the need of a new one if worthy enough
		_owners[i]->DeletePossession(this);

	}*/
}

void UOOwnable::DestroyOwnable() {

}
