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

vector<UOEntity*> UOOwnable::GetGrabbers() {
	return _grabbers;
}

void UOOwnable::AddGrabber(UOEntity* g) {
	_grabbers.push_back(g);
}

void UOOwnable::RemoveGrabber(UOEntity* rg) {
	int i = 0;
	for (UOEntity* g : _grabbers) {
		if (g == rg)
			_grabbers.erase(_grabbers.begin() + i);
		i++;
	}
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


/*void UOOwnable::SpawnOwnable(UOOwnable* o, UItem* spawner) {
	FActorSpawnParameters SpawnParams;
	if (o->GetItemName() == "hammer") {
		//spawner->GetWorld()->SpawnActor<AActor>(APlotGenerator::GetHammer(), spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "hoe") {
		//spawner->GetWorld()->SpawnActor<AActor>(APlotGenerator::BP_Hoe, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "pickaxe") {
		//spawner->GetWorld()->SpawnActor<AActor>(APlotGenerator::BP_Pickaxe, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "rake") {
		//spawner->GetWorld()->SpawnActor<AActor>(APlotGenerator::BP_Rake, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "sword") {
		//spawner->GetWorld()->SpawnActor<AActor>(APlotGenerator::BP_Sword, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "leaflesstree") {
		//spawner->GetWorld()->SpawnActor<AActor>(APlotGenerator::BP_LeaflessTree, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
}*/

FVector UOOwnable::RandomDisplacementVector(int radius) {
	return FVector(rand() % (2 * radius) - radius, rand() % (2 * radius) - radius, 0);
}