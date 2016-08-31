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

	if (!_isDestroyed) {

		CastNotify(this, damager, ENotify::N_Damaged);

		_integrity -= damage;
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, "damage: " + FString::SanitizeFloat(damage));
		_attacker = damager;

		UDestructibleComponent* targetDestructible = GetOwner()->FindComponentByClass<UDestructibleComponent>();

		if (targetDestructible)
			targetDestructible->ApplyRadiusDamage(damage, collisionPos, 50, 0.01, false);

		if (_integrity <= 0)
			DestroyEdification(damager);
	}
}


// NOTIFIES OWNER
// SETS OR MODIFIES RELATIONSHIP WITH DAMAGER
// GENERATES REPORT
// ELIMINATES POSSESSION

void UOEdification::IHaveBeenDestroyedBySomeone(UOEntity* damager)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("I have been destroyed by " + damager->GetOwner()->GetName()));

	//  R E A C T I V I T Y
	FVector start = GetOwner()->GetActorLocation();
	FVector end = start;
	TArray<FHitResult> outHits;

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, GetOwner());
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	GetOwner()->GetWorld()->SweepMultiByChannel(
		outHits,
		start,
		end,
		FQuat(),
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(_NOTIFICATION_RADIUS),
		RV_TraceParams
		);

	for (FHitResult hr : outHits) {
		UOEntity* entity = hr.GetActor()->FindComponentByClass<UOEntity>();
		if (entity->IsInSight(GetOwner()))
			entity->OwnableNotify(this, damager, UItem::_NotifyTag::destroyed, false, UItem::GenerateNotifyID(this, damager, UItem::_NotifyTag::destroyed));
	}*/


	//   P L O T S
	//	For each owner, check existing ownership and relation with damager, change ontological relation if required and send reports

	for(UOEntity* o : GetOwners()) {

		OOwnership* ownership = o->GetOwnershipWith(this);
		ORelation* relation = o->GetRelationWith(damager);

		if (!relation) {
			o->AddRelationship(new ORelation(o, damager));
			relation = o->GetRelationWith(damager);
		}

		relation->ChangeAppreciation(-ownership->GetWorth());
		if (relation->GetAppreciation() <= ORelation::LOW_APPRECIATION)
			o->SendReport(new Report(relation, TypeOfPlot::aggressive, this));

		// NOTIFY ABSENSE OF HOME
		o->SendReport(new Report(ownership, TypeOfPlot::resources, damager));
		
	}
}

void UOEdification::DestroyEdification(UOEntity* damager) {

	_isDestroyed = true;

	if (GetOwner()->FindComponentByClass<UDestructibleComponent>()) {

		UParticleSystemComponent* part = GetOwner()->FindComponentByClass<UParticleSystemComponent>();
		if (part)
			part->ActivateSystem();

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







