// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "Ontology/OEntity.h"
#include "BasePlot.h"

UOEntity::UOEntity() {
	_personality = new OPersonality();
}

UOEntity::UOEntity(OPersonality* personality) {
	_personality = personality;
}

void UOEntity::BeginPlay() {

	Super::BeginPlay();
	
	for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
		plotGenerator = *Itr;
}

void UOEntity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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


vector<ORelation*> UOEntity::GetRelationships() {
	return _relationships;
}
vector<OOwnership*> UOEntity::GetPossessions() {
	return _possessions;
}
vector<OTerritory*> UOEntity::GetTerritories() {
	return _landlord;
}

OPersonality* UOEntity::GetPersonality(){
	return _personality;
}

int UOEntity::GetNotoriety() {
	return _notoriety;
}

bool UOEntity::GetIsDead() {
	return _isDead;
}

void UOEntity::AddRelationship(ORelation* newRelation) {
	_relationships.push_back(newRelation);
}
void UOEntity::AddRelationship(UOEntity* newEntity) {
	_relationships.push_back(new ORelation(this, newEntity, _personality->GetSocial(), newEntity->GetNotoriety(), _personality->GetMaxValue() - _personality->GetBraveness()));
}
void UOEntity::AddPossession(OOwnership* newPossession) {
	_possessions.push_back(newPossession);
	newPossession->GetOwnable()->AddOwner(this);
}
void UOEntity::AddPossession(UOOwnable* newOwnable) {
	_possessions.push_back(new OOwnership(this, newOwnable, _personality->GetMaterialist()));
	newOwnable->AddOwner(this);
}
void UOEntity::AddTerritory(OTerritory* newTerritory) {
	_landlord.push_back(newTerritory);
}

void UOEntity::AddDesire(OOwnership * newOwnership)
{
	_materialDesires.push_back(newOwnership);
}

void UOEntity::AddDesire(UOOwnable * newOwnable)
{
	_materialDesires.push_back(new OOwnership(this, newOwnable, _personality->GetMaterialist()));
}

ORelation* UOEntity::GetRelationWith(UOEntity * other)
{
	for (int i = 0; i < _relationships.size(); i++) {
		if (_relationships[i]->GetOtherEntity() == other)
			return _relationships[i];
	}

	return nullptr;
}

OOwnership* UOEntity::GetOwnershipWith(UOOwnable * other)
{
	for (int i = 0; i < _possessions.size(); i++) {
		if (_possessions[i]->GetOwnable() == other)
			return _possessions[i];
	}

	return nullptr;
}

void UOEntity::DeleteRelation(UOEntity * relative)
{
	int i = 0;

	while (i < _relationships.size()) {

		if (_relationships[i]->GetOtherEntity() == relative) {
			break;
		}
		else i++;
	}

	_relationships.erase(_relationships.begin() + i);
}

void UOEntity::DeletePossession(UOOwnable * possession)
{
	int i = 0;

	while (i < _possessions.size()) {

		if (_possessions[i]->GetOwnable() == possession) {
			break;
		}
		else i++;
	}

	_possessions.erase(_possessions.begin() + i);
}

void UOEntity::DeleteDesire(UOOwnable * desire)
{
	int i = 0;

	while (i < _materialDesires.size()) {

		if (_materialDesires[i]->GetOwnable() == desire) {
			break;
		}
		else i++;
	}

	_materialDesires.erase(_materialDesires.begin() + i);
}


bool UOEntity::IsInSight(AActor* actor) {
	return true;
}

void UOEntity::OwnableNotify(UOOwnable* ownable, UOEntity* entity, UItem::_NotifyTag tag, bool grito, string notifyID) {

}
void UOEntity::EntityNotify(UOEntity* pasiva, UOEntity* activa, UItem::_NotifyTag tag, bool grito, string notifyID) {

}



void UOEntity::ReceiveDamage(float damage, UOEntity * damager)
{
	if (!_isDead && _canBeDamaged) {

		_integrity -= damage;
		_attacker = damager;
		_canBeDamaged = false;

		if (_integrity < MIN_INTEGRITY) {
			Die();
			IHaveBeenKilledBySomeone(damager);
		}
	}
}

// BEFORE SENDING, ELEGIBLE TYPE MUST BE CHECKED WITH PERSONALITY
// CONFIRMED REPORTS ARE PRINTED ON SCREEN
void UOEntity::SendReport(Report * newReport)
{
	if (CheckValidPersonality(newReport->GetType()))
		plotGenerator->AddReportToLog(newReport);
}

void UOEntity::IHaveBeenKilledBySomeone(UOEntity * killer)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I have been killed by " + killer->GetOwner()->GetActorLabel()));

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
		if (entity->IsInSight(GetOwner())) {
			// Generate Notification
		}	
	}


	//   P L O T S
	//	Search in relationships for those who appreciate entity, change ontology if required and send reports

	vector<ORelation*> relations = GetRelationships();

	for (ORelation* o : relations) {

		ORelation* relationFromOther = o->GetOtherEntity()->GetRelationWith(this);

		if (relationFromOther){

			if (relationFromOther->GetAppreciation() > relationFromOther->LOW_APPRECIATION) {
				ORelation* relationWithKiller = o->GetOtherEntity()->GetRelationWith(killer);

				if (!relationWithKiller) {
					o->GetOtherEntity()->AddRelationship(new ORelation(o->GetOtherEntity(), killer));
					relationWithKiller = o->GetOtherEntity()->GetRelationWith(killer);
				}

				relationWithKiller->SetAppreciation(-relationFromOther->GetAppreciation());

				if (relationWithKiller->GetAppreciation() < relationWithKiller->LOW_APPRECIATION)
					o->GetOtherEntity()->SendReport(new Report(relationWithKiller, BasePlot::TypeOfPlot::aggressive, this));
			}
			o->GetOtherEntity()->DeleteRelation(this);
		}
	}
}

void UOEntity::Die() {

	ACharacter* character = dynamic_cast<ACharacter*>(GetOwner());
	character->GetMesh()->SetSimulatePhysics(true);

	_isDead = true;
}

bool UOEntity::CheckValidPersonality(BasePlot::TypeOfPlot type) {

	switch (type) {

	case BasePlot::TypeOfPlot::aggressive:
		if (_personality->GetAggressiveness() < 25 || _personality->GetBraveness() < 25) return false;

	case BasePlot::TypeOfPlot::possessive:
		if (_personality->GetMaterialist() < 25 || _personality->GetAggressiveness() < 25) return false;

	case BasePlot::TypeOfPlot::resources: 
		return true;

	case BasePlot::TypeOfPlot::thankful:
		if (_personality->GetKindness() < 25 || _personality->GetSocial() < 25) return false;

	case BasePlot::TypeOfPlot::preventive:
		return true;
	}

	return true;
}

