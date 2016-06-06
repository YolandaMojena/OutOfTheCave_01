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

		if (ownership && relation) {

			relation->SetAppreciation(relation->GetAppreciation() - ownership->GetWorth());
			o->SendReport(new Report(relation, { BasePlot::TypeOfPlot::aggressive, BasePlot::TypeOfPlot::possessive }, this));
			//o->DeletePossession(this);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inexisting relationship or ownership"));
	}

	/*for (UOEntity* o : _owners) {
		for (OOwnership* ow : o->GetPossessions()) {
			if (this == ow->GetOwnable()) {
		
			}
		}
	}*/
}

void UOOwnable::DestroyOwnable() {

}
