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
	for (auto i : GetRelationships()) {
		if (i->GetOtherEntity() == relative) {
			GetRelationships().erase(remove(GetRelationships().begin(), GetRelationships().end(), i), GetRelationships().end());
			break;
		}	
	}
}

void UOEntity::DeletePossession(UOOwnable * possession)
{
	for (auto i : GetPossessions()) {
		if (i->GetOwnable() == possession)
			GetPossessions().erase(remove(GetPossessions().begin(), GetPossessions().end(), i), GetPossessions().end());
			break;
		}
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
// CONFIRMED REPORTS ARE ALSO PRINTED ON SCREEN
void UOEntity::SendReport(Report * newReport)
{

	vector<BasePlot::TypeOfPlot> reportedTypes = newReport->GetTypes();

	for (int i = 0; i < reportedTypes.size(); i++) {

	}

	

	///int kind, int brave, int aggressive, int social, int materialist

	//plotGenerator->AddReportToLog(newReport);

}

void UOEntity::IHaveBeenKilledBySomeone(UOEntity * killer)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I have been killed by someone"));
}

void UOEntity::Die() {

}

bool UOEntity::CheckValidPersonality(BasePlot::TypeOfPlot type) {

	/*switch (type) {

	case BasePlot::TypeOfPlot::aggressive:
		if (_personality->GetAggressiveness() < 50 && _personality->GetBraveness() < 50) return false;

	case BasePlot::TypeOfPlot::possesive:
		if (_personality->GetMaterialist() < 50 && _personality->GetAggressiveness() < 50) return false;

	case BasePlot::TypeOfPlot::resources: 
		return true;

	case BasePlot::TypeOfPlot::thankful:
		if (_personality->GetKindness() < 50 && _personality->GetSocial() < 50) return false;

	case BasePlot::TypeOfPlot::preventive:
		return true;

	}*/

	return true;
}

