// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "Ontology/OEntity.h"

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
		if (_possessions[i]->GetItem() == other)
			return _possessions[i];
	}

	return nullptr;
}

void UOEntity::DeleteRelation(UOEntity * relative)
{
	for (auto i : GetRelationships()) {
		if(i->GetOtherEntity() == relative)
			GetRelationships().erase(remove(GetRelationships().begin(), GetRelationships().end(), i), GetRelationships().end());
	}
}

void UOEntity::DeletePossession(UOOwnable * possession)
{
	for (auto i : GetPossessions()) {
		if (i->GetItem() == possession)
			GetPossessions().erase(remove(GetPossessions().begin(), GetPossessions().end(), i), GetPossessions().end());
	}
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

void UOEntity::SendReport(Report * newReport)
{
	if (newReport->GetTag() == Report::ReportTag::relation) {

		FString report = "Report from entity " + newReport->GetReportEntity()->GetOwner()->GetActorLabel() + " about entity " + newReport->GetTargetEntity()->GetOwner()->GetActorLabel() + ". Reports a change caused by " + newReport->GetMotivation()->GetOwner()->GetActorLabel();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, report);
	}
	else if (newReport->GetTag() == Report::ReportTag::ownership) {

		FString report = "Report from entity " + newReport->GetReportEntity()->GetOwner()->GetActorLabel() + " about ownable " + newReport->GetTargetOwnable()->GetOwner()->GetActorLabel() + ". Reports a change caused by: " + newReport->GetMotivation()->GetOwner()->GetActorLabel();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, report);
	}

	// BEFORE SENDING, TYPE MUST BE DETERMINED BY PERSONALITY, IN THIS CLASS OR FROM THE CAUSE OF THE REPORT

	plotGenerator->AddReportToLog(newReport);

}

void UOEntity::IHaveBeenKilledBySomeone(UOEntity * killer)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I have been killed by someone"));
}

void UOEntity::Die() {

}

