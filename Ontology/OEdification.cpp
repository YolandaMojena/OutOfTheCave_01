// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OEdification.h"
#include "Village.h"
#include "Ontology/OEntity.h"

UOEdification::UOEdification(const FObjectInitializer& ObjectInitializer) : UOOwnable() {

	destructibleComp = ObjectInitializer.CreateDefaultSubobject<UDestructibleComponent>(this->GetOwner(), TEXT("DM"));
}

UOEdification::UOEdification()
{}

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

void UOEdification::ReceiveDamage(float damage, UOEntity* damager) {

	if (!_isDestroyed) {

		_integrity -= damage;
		_attacker = damager;
		//_canBeDamaged = false;

		if (_integrity <= 0) {
			_isDestroyed = true;
			DestroyEdification();
			//IHaveBeenDestroyedBySomeone(damager);
		}
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
		if (relation->GetAppreciation() <= relation->LOW_APPRECIATION)
			o->SendReport(new Report(relation, BasePlot::TypeOfPlot::aggressive, this));

		//o->SendReport(new Report(relation, BasePlot::TypeOfPlot::possessive, this));

		//o->DeletePossession(this);
	}*/
}

void UOEdification::DestroyEdification() {

	UParticleSystemComponent* part = GetOwner()->FindComponentByClass<UParticleSystemComponent>();

	if (part) {
		part->ActivateSystem();
		RebuildEdification();
	}
}

bool UOEdification::RebuildEdification() {

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DestructibleMesh(TEXT("DestructibleMesh'/Game/MyContent/humbleHouse_DM.humbleHouse_DM'"));
	originalMesh = DestructibleMesh.Object;

	GetOwner()->FindComponentByClass<UDestructibleComponent>()->DestroyComponent();
	destructibleComp->AttachTo(GetOwner()->GetRootComponent());

	return true;
}







