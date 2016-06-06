// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "Ontology/OEntity.h"
#include "BasePlot.h"

UOEntity::UOEntity() {
	
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
}
void UOEntity::AddPossession(UOOwnable* newOwnable) {
	_possessions.push_back(new OOwnership(this, newOwnable, _personality->GetMaterialist()));
}
void UOEntity::AddTerritory(OTerritory* newTerritory) {
	_landlord.push_back(newTerritory);
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

	while (i < GetRelationships().size()) {

		if (GetRelationships()[i]->GetOtherEntity() == relative) {
			GetRelationships().erase(remove(GetRelationships().begin(), GetRelationships().end(), GetRelationships()[i]), GetRelationships().end());
			break;
		}
		else i++;
	}
}

void UOEntity::DeletePossession(UOOwnable * possession)
{
	int i = 0;

	while (i < GetPossessions().size()) {

		if (GetPossessions()[i]->GetOwnable() == possession) {
			GetPossessions().erase(remove(GetPossessions().begin(), GetPossessions().end(), GetPossessions()[i]), GetPossessions().end());
			break;
		}
		else i++;
	}
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
			_isDead = true;
			Die();
			IHaveBeenKilledBySomeone(damager);
		}
	}
}

// BEFORE SENDING, ELEGIBLE TYPE MUST BE CHECKED WITH PERSONALITY
// CONFIRMED REPORTS ARE PRINTED ON SCREEN
void UOEntity::SendReport(Report * newReport)
{
	vector<BasePlot::TypeOfPlot> reportedTypes = newReport->GetTypes();

	int i = 0;
	while (i < newReport->GetTypes().size()) {

		if (!CheckValidPersonality(reportedTypes[i]))
			newReport->RemoveTagFromReport(reportedTypes[i]);
		else i++;
	}

	if (newReport->GetTypes().size() > 0) {
		newReport->PrintReport(newReport);
		plotGenerator->AddReportToLog(newReport);
	}
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

	for (ORelation* o : GetRelationships()) {

		ORelation* relationFromOther = o->GetOtherEntity()->GetRelationWith(this);
		ORelation* relationWithKiller = o->GetOtherEntity()->GetRelationWith(killer);

		// Study state of relationship
		if (relationFromOther->GetAppreciation() > 75 && relationWithKiller->GetAppreciation() < relationFromOther->GetAppreciation()) {

			relationWithKiller->SetAppreciation(relationFromOther->GetAppreciation() - relationWithKiller->GetAppreciation());
			o->GetOtherEntity()->SendReport(new Report(relationWithKiller, { BasePlot::TypeOfPlot::aggressive }, this));
			//o->DeleteGetOtherEntity()->DeleteRelation(this);
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Entities are not well-related"));
	}
}

void UOEntity::Die() {

	ACharacter* character = dynamic_cast<ACharacter*>(GetOwner());
	character->GetMesh()->SetSimulatePhysics(true);
}

bool UOEntity::CheckValidPersonality(BasePlot::TypeOfPlot type) {

	switch (type) {

	case BasePlot::TypeOfPlot::aggressive:
		if (_personality->GetAggressiveness() < 50 || _personality->GetBraveness() < 50) return false;

	case BasePlot::TypeOfPlot::possessive:
		if (_personality->GetMaterialist() < 50 || _personality->GetAggressiveness() < 50) return false;

	case BasePlot::TypeOfPlot::resources: 
		return true;

	case BasePlot::TypeOfPlot::thankful:
		if (_personality->GetKindness() < 50 || _personality->GetSocial() < 50) return false;

	case BasePlot::TypeOfPlot::preventive:
		return true;
	}

	return true;
}

