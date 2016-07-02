// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/OEntity.h"
#include "Ontology/OCivilian.h"
#include "Ontology/OOwnership.h"
#include "Ontology/ORelation.h"
#include "NarrativeGeneration/Report.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "Ontology/OOwnable.h"



UOOwnable::UOOwnable() {}
UOOwnable::~UOOwnable(){}

void UOOwnable::BeginPlay() {

	Super::BeginPlay();

	//EXAMPLE OF OBJECT ITERATOR
	/*for (TObjectIterator<UOCivilian> Itr; Itr; ++Itr){
	}*/

	for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
		_plotGenerator = *Itr;

	if (_rarity == Rarity::mythic || _rarity == Rarity::rare)
		_plotGenerator->AddValuable(this);
}

void UOOwnable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

vector<UOEntity*> UOOwnable::GetOwners() {
	return _owners;
}
void UOOwnable::AddOwner(UOEntity* e) {
	_owners.push_back(e);
}

Rarity UOOwnable::GetRarity() {
	return _rarity;
}
int UOOwnable::GetRarityAsInt() {
	return (int)_rarity;
}

bool UOOwnable::GetIsGrabbed()
{
	return _isGrabbed;
}

void UOOwnable::SetIsGrabbed(bool grabbed)
{
	_isGrabbed = grabbed;
}

void UOOwnable::IHaveBeenStolenBySomeone(UOEntity * potentialOwner, UOEntity * buggler)
{
	for (UOEntity* e : _owners) {
		OOwnership* ownership = e->GetOwnershipWith(this);

		if (ownership->GetWorth() > 50) {

			ORelation* relation = e->GetRelationWith(buggler);

			if (relation)
				relation->SetAppreciation(-ownership->GetWorth());

			if (!relation || relation->GetAppreciation() < 50) {

				Report* newReport = new Report(e->GetOwnershipWith(this), TypeOfPlot::possessive, buggler);
			}	
		}

	}
}


