// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/OEntity.h"
#include "Ontology/OCivilian.h"
#include "Ontology/OOwnership.h"
#include "Ontology/ORelation.h"
#include "NarrativeGeneration/Report.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "Ontology/OOwnable.h"



UOOwnable::UOOwnable() {
	/*ConstructorHelpers::FObjectFinder<UBlueprint> HammerBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Hammer.Hammer'"));
	if (HammerBlueprint.Object) {
		BP_Hammer = (UClass*)HammerBlueprint.Object->GeneratedClass;
	}
	ConstructorHelpers::FObjectFinder<UBlueprint> HoeBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Hoe.Hoe'"));
	if (HoeBlueprint.Object) {
		BP_Hoe = (UClass*)HoeBlueprint.Object->GeneratedClass;
	}
	ConstructorHelpers::FObjectFinder<UBlueprint> PickaxeBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Pickaxe.Pickaxe'"));
	if (PickaxeBlueprint.Object) {
		BP_Pickaxe = (UClass*)PickaxeBlueprint.Object->GeneratedClass;
	}
	ConstructorHelpers::FObjectFinder<UBlueprint> RakeBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Rake.Rake'"));
	if (RakeBlueprint.Object) {
		BP_Rake = (UClass*)RakeBlueprint.Object->GeneratedClass;
	}
	ConstructorHelpers::FObjectFinder<UBlueprint> SwordBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Sword2.Sword2'"));
	if (SwordBlueprint.Object) {
		BP_Sword = (UClass*)SwordBlueprint.Object->GeneratedClass;
	}
	ConstructorHelpers::FObjectFinder<UBlueprint> LeaflessTreeBlueprint(TEXT("Blueprint'/Game/Blueprints/LeaflessTree.LeaflessTree'"));
	if (LeaflessTreeBlueprint.Object) {
		BP_LeaflessTree = (UClass*)LeaflessTreeBlueprint.Object->GeneratedClass;
	}*/
}
UOOwnable::~UOOwnable(){}

void UOOwnable::BeginPlay() {

	Super::BeginPlay();

	
	//EXAMPLE OF OBJECT ITERATOR
	/*for (TObjectIterator<UOCivilian> Itr; Itr; ++Itr){
	}*/

	/*for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
		_plotGenerator = *Itr;*/

	if (_rarity == Rarity::mythic || _rarity == Rarity::rare)
		_plotGenerator->AddValuable(this);

	_plotGenerator->allOwnables.Add(this);
}

void UOOwnable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

vector<UOEntity*> UOOwnable::GetOwners() {
	return _owners;
}
void UOOwnable::AddOwner(UOEntity* e) {
	if(e->IsValidItem())
		_owners.push_back(e);
}
void UOOwnable::RemoveOwner(UOEntity* owner) {
	if (!owner->IsValidItem())
		return;

	int i = 0;
	for (UOEntity* o : _owners) {
		if (!o->IsValidItem())
			continue;

		if (o == owner)
			_owners.erase(_owners.begin() + i);
		i++;
	}
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

vector<UOEntity*> UOOwnable::GetGrabbers() {
	return _grabbers;
}

void UOOwnable::AddGrabber(UOEntity* g) {
	if (!g->IsValidItem())
		return;
	
	_grabbers.push_back(g);
}

void UOOwnable::RemoveGrabber(UOEntity* rg) {
	if (!rg->IsValidItem())
		return;

	int i = 0;
	for (UOEntity* g : _grabbers) {
		if (!g->IsValidItem())
			continue;

		if (g == rg)
			_grabbers.erase(_grabbers.begin() + i);
		i++;
	}
}

/*int UOOwnable::GetIntrinsicValue() {
	int value = 0;
	value += 10 * GetRarityAsInt();
	OntologicFunctions* ontf = new OntologicFunctions();
	for (int i = OntologicFunctions::AffordableUse::weapon; i < OntologicFunctions::AffordableUse::build; i++) {
		value += ontf->GetAffordance((OntologicFunctions::AffordableUse)i, present, this) * 5.f / 100.f;
	}
	return value;
}*/

void UOOwnable::IHaveBeenStolenBySomeone(UOEntity * potentialOwner, UOEntity * buggler)
{
	if (!potentialOwner->IsValidItem() || !buggler->IsValidItem())
		return;

	for (UOEntity* e : _owners) {
		if (!e->IsValidItem())
			continue;

		OOwnership* ownership = e->GetOwnershipWith(this);

		if (ownership && ownership->GetWorth() > 50) {

			ORelation* relation = e->GetRelationWith(buggler);

			if (!relation)
				relation = e->AddRelationship(buggler);

			if (relation) {
				relation->ChangeAppreciation(-ownership->GetWorth());

				if (relation->GetAppreciation() < 50) {

					if (e == potentialOwner && ownership->GetWorth() > 60)
						e->SendReport(new Report(e->GetOwnershipWith(this), TypeOfPlot::possessive, buggler));

					e->SendReport(new Report(e->GetRelationWith(buggler), TypeOfPlot::aggressive, this));
				}
			}
		}
	}
}

FVector UOOwnable::RandomDisplacementVector(int radius) {
	return FVector(rand() % (2 * radius) - radius, rand() % (2 * radius) - radius, 0);
}