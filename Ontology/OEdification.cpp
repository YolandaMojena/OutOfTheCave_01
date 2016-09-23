// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OEdification.h"
#include "Village.h"
#include "Ontology/OEntity.h"
#include "Ontology/OTree.h"

UOEdification::UOEdification() : UOOwnable() {
}

UOEdification::~UOEdification()
{}

// Called when the game starts
void UOEdification::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FindVillage();
}

bool UOEdification::GetIsDestroyed() {
	return _isDestroyed;
}


void UOEdification::FindVillage() {
	if (villageID != 0) {
		for (TActorIterator<AVillage> ActorItr(GetOwner()->GetWorld()); ActorItr; ++ActorItr) {
			AVillage* potentialVillage = *ActorItr;
			if (potentialVillage->villageID == villageID)
				_village = potentialVillage;
		}
	}
}

void UOEdification::ReceiveDamage(float damage, UOEntity* damager, FVector collisionPos) {
	//->IsValidItem() inside the main <if>

	if (!_isDestroyed) {
		_integrity -= damage;

		if (damager->IsValidItem()) {
			_attacker = damager;

			if (_integrity <= 0)
				DestroyEdification(damager);
			else
				CastNotify(this, damager, ENotify::N_Damaged);
		}
	}
	
	UDestructibleComponent* targetDestructible = GetOwner()->FindComponentByClass<UDestructibleComponent>();
		if (targetDestructible)
			targetDestructible->ApplyRadiusDamage(damage, collisionPos, 50, 0.01, false);
}


// NOTIFIES OWNER
// SETS OR MODIFIES RELATIONSHIP WITH DAMAGER
// GENERATES REPORT
// ELIMINATES POSSESSION

void UOEdification::IHaveBeenDestroyedBySomeone(UOEntity* damager)
{
	if (!damager->IsValidItem())
		return;
	//   P L O T S
	//	For each owner, check existing ownership and relation with damager, change ontological relation if required and send reports

	for(UOEntity* o : GetOwners()) {
		if (!o->IsValidItem())
			continue;

		OOwnership* ownership = o->GetOwnershipWith(this);
		ORelation* relation = o->GetRelationWith(damager);
		if (!relation) {
			relation = o->AddRelationship(damager);
		}

		if (relation) {
			relation->ChangeAppreciation(-ownership->GetWorth());
			if (relation->GetAppreciation() <= ORelation::LOW_APPRECIATION)
				o->SendReport(new Report(relation, TypeOfPlot::aggressive, this));

			CastNotify((UItem*)this, damager, ENotify::N_Obliterated);

			// NOTIFY ABSENSE OF HOME
			o->SendReport(new Report(ownership, TypeOfPlot::resources, damager));
		}
	}
}

void UOEdification::DestroyEdification(UOEntity* damager) {
	//->IsValidItem() in between

	_isDestroyed = true;

	if (GetOwner()->FindComponentByClass<UDestructibleComponent>()) {

		UParticleSystemComponent* part = GetOwner()->FindComponentByClass<UParticleSystemComponent>();
		if (part)
			part->ActivateSystem();

		if(damager->IsValidItem())
			IHaveBeenDestroyedBySomeone(damager);
	}
	else if (IsA<UOTree>()) {
		((UOTree*)this)->SpawnLeaflessTree();
	}
}

void UOEdification::RebuildEdification(float addedValue) {

	_accumulatedTime += addedValue;

	if (_accumulatedTime >= timeToRebuild) {

		((ARebuildableEdification*)GetOwner())->RebuildEdification();
		_isDestroyed = false;
		_integrity = 100;
		_accumulatedTime = 0;
	}
}







