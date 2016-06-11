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

	//EXAMPLE OF OBJECT ITERATOR
	/*for (TObjectIterator<UOCivilian> Itr; Itr; ++Itr){
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

vector<UOEntity*> UOOwnable::GetOwners() {
	return _owners;
}
void UOOwnable::AddOwner(UOEntity* e) {
	_owners.push_back(e);
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

// NOTIFIES OWNER
// SETS OR MODIFIES RELATIONSHIP WITH DAMAGER
// GENERATES REPORT
// ELIMINATES POSSESSION

void UOOwnable::IHaveBeenDestroyedBySomeone(UOEntity* damager)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("I have been destroyed by " + damager->GetOwner()->GetName()));

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
	}


	//   P L O T S
	//	For each owner, check existing ownership and relation with damager, change ontological relation if required and send reports

	for (UOEntity* o : _owners) {

		OOwnership* ownership = o->GetOwnershipWith(this);
		ORelation* relation = o->GetRelationWith(damager);

		if (!relation) {
			o->AddRelationship(new ORelation(o, damager));
			ORelation* relation = o->GetRelationWith(damager);
		}

		relation->ChangeAppreciation(-ownership->GetWorth());
		if(relation->GetAppreciation() <= relation->LOW_APPRECIATION)
			o->SendReport(new Report(relation, BasePlot::TypeOfPlot::aggressive, this));

		//o->SendReport(new Report(relation, BasePlot::TypeOfPlot::possessive, this));

		//o->DeletePossession(this);
	}
}

void UOOwnable::DestroyOwnable() {

}
