// Fill out your copyright notice in the Description page of Project Settings.
#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OEdification.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "EntityAIController.h"
#include "Ontology/OEntity.h"
#include "Ontology/Ocivilian.h"
#include "BasePlot.h"

UOEntity::UOEntity() {
	_personality = new OPersonality();
	_deadOwnable = CreateDefaultSubobject<UOOwnable>(TEXT("DeadOwnable"));
	HitFunc.BindUFunction(this, "OnOverlapBegin");
}

UOEntity::UOEntity(OPersonality* personality) {
	_personality = personality;
	_deadOwnable = CreateDefaultSubobject<UOOwnable>(TEXT("DeadOwnable"));

	HitFunc.BindUFunction(this, "OnOverlapBegin");
}

void UOEntity::BeginPlay() {

	Super::BeginPlay();
	
	if (!IsPlayer) {
		//((ACharacter*)GetOwner())->GetMesh()->SetAllBodiesBelowSimulatePhysics(((ACharacter*)GetOwner())->GetMesh()->GetBoneName(1), true);
		for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
			_plotGenerator = *Itr;
	}
}

void UOEntity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// G E T T E R S

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
void UOEntity::ChangeNotoriety(int value) {
	_notoriety += value;
}

bool UOEntity::GetIsDead() {
	return _isDead;
}

UOEntity* UOEntity::GetMainPlotEntity() {
	return _mainPlotEntity;
}

int UOEntity::GetStrength() {
	return _strength;
}

UItem* UOEntity::GetGrabbedItem() {
	return _grabbedItem;
}
bool UOEntity::HasGrabbedItem() {
	return _grabbedItem != nullptr;
}

/*Graph* UOEntity::GetBrain() {
	return &_brain;
}*/
ERace UOEntity::GetRace()
{
	return _race;
}
FString UOEntity::GetRaceString()
{
	FString raceName;
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERace"), true);
	if (EnumPtr) raceName = EnumPtr->GetEnumName((int32)_race);
	raceName.RemoveFromStart("R_");
	return raceName;
}
void UOEntity::SetRace(ERace race)
{
	_race = race;
}


// R E L A T I O N S

void UOEntity::AddRelationship(ORelation* newRelation) {
	_relationships.push_back(newRelation);
	DeletePotentitalRelationship(newRelation->GetOtherEntity());
}
void UOEntity::AddRelationship(UOEntity* newEntity) {
	_relationships.push_back(new ORelation(this, newEntity, _personality->GetSocial(), newEntity->GetNotoriety(), _personality->GetMaxValue() - _personality->GetBraveness()));
	DeletePotentitalRelationship(newEntity);
}
void UOEntity::AddPotentialRelationship(UOEntity * newEntity)
{
	_potentialRelationships.push_back(newEntity);
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
	bool contains = false;
	while (i < _relationships.size()) {
		if (_relationships[i]->GetOtherEntity() == relative) {
			contains = true;
			break;
		}
		else i++;
	}
	if(contains) _relationships.erase(_relationships.begin() + i);
}

void UOEntity::DeletePotentitalRelationship(UOEntity * entity)
{
	int i = 0;
	bool contains = false;

	while (i < _potentialRelationships.size()) {
		if (_potentialRelationships[i] == entity) {
			contains = true;
			break;
		}
		else i++;
	}
	if(contains) _potentialRelationships.erase(_potentialRelationships.begin() + i);
}

void UOEntity::DeletePossession(UOOwnable * possession)
{
	int i = 0;
	bool contains = false;
	while (i < _possessions.size()) {
		if (_possessions[i]->GetOwnable() == possession) {
			contains = true;
			break;
		}
		else i++;
	}
	if(contains) _possessions.erase(_possessions.begin() + i);
}

void UOEntity::DeleteDesire(UOOwnable * desire)
{
	int i = 0;
	bool contains = false;

	while (i < _materialDesires.size()) {

		if (_materialDesires[i]->GetOwnable() == desire) {
			contains = true;
			break;
		}
		else i++;
	}
	if(contains) _materialDesires.erase(_materialDesires.begin() + i);
}

bool UOEntity::DoesOwn(UOOwnable* ownable) {
	for (OOwnership* ownership : _possessions)
		if (ownable == ownership->GetOwnable())
			return true;
	return false;
}
bool UOEntity::DoesOwn(UItem* item) {
	UOOwnable* ownable = item->GetOwner()->FindComponentByClass<UOOwnable>();
	if (ownable) {
		for (OOwnership* ownership : _possessions)
			if (ownable == ownership->GetOwnable())
				return true;
	}
	return false;
}




// N O T I F Y   S Y S T E M

bool UOEntity::IsInSight(AActor* actor) {
	return true;
}

void UOEntity::OwnableNotify(UOOwnable* ownable, UOEntity* entity, UItem::_NotifyTag tag, bool grito, string notifyID) {

}
void UOEntity::EntityNotify(UOEntity* pasiva, UOEntity* activa, UItem::_NotifyTag tag, bool grito, string notifyID) {

}




// E V E N T S


void UOEntity::ReceiveDamage(float damage, UOEntity * damager)
{
	if (!_isDead) {

		_integrity -= damage;
		_attacker = damager;

		if (_integrity < MIN_INTEGRITY) {
			IHaveBeenKilledBySomeone(damager);
			Die();
		}
	}
}

// BEFORE SENDING, ELEGIBLE TYPE MUST BE CHECKED WITH PERSONALITY
void UOEntity::SendReport(Report* newReport)
{
	if (CheckValidPersonality(newReport->GetType())) {
		_plotGenerator->AddReportToLog(newReport);
	}
}

// When an entity dies he turns into an ownable
// Those who appreciate the entity obtain the possession of that ownable where worth = appreciation and lose the relationship
// If the entity is involved in a plot, the most notorious entity ascends to main entity
void UOEntity::Die() {

	_isDead = true;

	ACharacter* character = (ACharacter*)GetOwner();
	character->GetMesh()->SetSimulatePhysics(true);
	character->GetMesh()->AttachTo(character->GetCapsuleComponent());
	character->GetCapsuleComponent()->AttachTo(character->GetMesh());

	if (IsA<UOCivilian>()) {
		((UOCivilian*)this)->currentIconPath = "";
		FText TestHUDText = NSLOCTEXT("FText Namespace", "Key", "");
		GetOwner()->FindComponentByClass<UTextRenderComponent>()->Text = TestHUDText;
	}
	/*
	// Add ownable
	GetOwner()->AddOwnedComponent(_deadOwnable);

	// Remove from relationships with others and add as possession
	for (ORelation* o : _relationships) {

		UOEntity* other = o->GetOtherEntity();
		ORelation* relationWithThis = o->GetOtherEntity()->GetRelationWith(this);

		if (relationWithThis) {
			int worth = relationWithThis->GetAppreciation();
			other->DeleteRelation(this);
			o->GetOtherEntity()->AddPossession(new OOwnership(other, _deadOwnable, worth));
		}
	}

	for (UOEntity* other : _potentialRelationships) {

		ORelation* relationWithThis = other->GetRelationWith(this);

		if (relationWithThis) {
			int worth = relationWithThis->GetAppreciation();
			other->DeleteRelation(this);
			other->AddPossession(new OOwnership(other, _deadOwnable, worth));
		}
	}
	
	// Handle plot state and main entity

	if (_currentState == State::plot) {

		if (_mainPlotEntity == this) {

			if (_currentPlots.size() > 0 && !_currentPlots[0]->GetIsExclusive()) {

				UOEntity* newMainEntity = new UOEntity();
				newMainEntity->ChangeNotoriety(0);

				for (UOEntity* entity : _currentPlots[0]->GetInvolvedInPlot()) {
					if (entity->GetNotoriety() >= newMainEntity->GetNotoriety())
						newMainEntity = entity;
				}

				for (UOEntity* entity : _currentPlots[0]->GetInvolvedInPlot()) {
					entity->SetMainPlotEntity(newMainEntity);
				}
			}
		}
	}

	GetOwner()->RemoveOwnedComponent(this);
	this->DestroyComponent();*/
}



// Generates notifications for reactiveness
// Generates Plot Reports and changes in ontology
// Deletes entity from the relationships lists of its relationships
void UOEntity::IHaveBeenKilledBySomeone(UOEntity * killer)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I have been killed by " + killer->GetOwner()->GetActorLabel()));

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
			// CHANGE FOR HIGH APPRECIATION
			if (relationFromOther->GetAppreciation() >= 0/*relationFromOther->LOW_APPRECIATION*/) {
				ORelation* relationWithKiller = relationFromOther->GetEntity()->GetRelationWith(killer);

				if (!relationWithKiller) {
					relationFromOther->GetEntity()->AddRelationship(new ORelation(relationFromOther->GetEntity(), killer));
					relationWithKiller = relationFromOther->GetEntity()->GetRelationWith(killer);
				}

				relationWithKiller->ChangeAppreciation(-relationFromOther->GetAppreciation());
				//if (relationWithKiller->GetAppreciation() < relationWithKiller->LOW_APPRECIATION)
				relationFromOther->GetEntity()->SendReport(new Report(relationWithKiller, TypeOfPlot::aggressive, this));
			}
			o->GetOtherEntity()->DeleteRelation(this);
		}
	}
}

void UOEntity::SetMainPlotEntity(UOEntity* mpe) {
	_mainPlotEntity = mpe;
}

APlotGenerator * UOEntity::GetPlotGenerator()
{
	return _plotGenerator;
}

vector<BasePlot*> UOEntity::GetCurrentPlots() {
	return _currentPlots;
}
BasePlot* UOEntity::GetCurrentPlot(){
	return _currentPlots[0];
}
void UOEntity::AddCurrentPlot(BasePlot* bp) {
	_currentPlots.push_back(bp);
}


bool UOEntity::CheckValidPersonality(TypeOfPlot type) {

	switch (type) {

	case TypeOfPlot::aggressive:
	//	if (_personality->GetAggressiveness() < 50 || _personality->GetBraveness() < 50) return false;
		return true;

	case TypeOfPlot::possessive:
	//	if (_personality->GetMaterialist() < 50 || _personality->GetAggressiveness() < 50) return false;
		return true;

	// Avery entity worries about it's home and basic needs 
	case TypeOfPlot::resources: 
		return true;

	case TypeOfPlot::thankful:
		if (_personality->GetKindness() < 50 || _personality->GetSocial() < 50) return false;
		return true;

	case TypeOfPlot::preventive:
		return true;
	}

	return true;
}




// S T A T E S

void UOEntity::SetIdleGraph(Graph* g) {
	_idleGraph = g;
}

UOEntity::State UOEntity::GetCurrentState() {
	return _currentState;
}

void UOEntity::SetState(State s, Graph* g) {
	if (s == State::idle && (_currentPlots.size() > 0 || _mainPlotEntity)) {
		s = State::plot;
	}
	_currentState = s;
	switch (_currentState) {
	case State::idle:
	{
		_brain = *_idleGraph;
		if (_brain.Peek()->nBlackboard.daytime >= 0) {
			while (_brain.Peek()->nBlackboard.daytime < _currentTime) {
				_brain.NextNode();
			}
		}
	}
		break;
	case State::plot:
	{	
		if (_currentPlots.size() > 0) {
			_brain = _currentPlots[0]->GetGraph();
			_mainPlotEntity = this;
			_plotGenerator->ChangeCurrentPlotsInAction(+1);
		}
		else {
			if (_mainPlotEntity) {
				_brain = GetMainPlotEntity()->GetCurrentPlot()->GetGraph();

				Node* comeToEntity = new Node();
				comeToEntity->SetNodeType(NodeType::goToItem);
				comeToEntity->SetActorA(_mainPlotEntity->GetOwner());
				_brain.AddInstantNode(comeToEntity);
				_brain.NextNode();
			}
		}
	}
		break;
	case State::react:
	{
		_brain = *g;
	}
		break;
	default:
		_brain = *_idleGraph;
	}

	_entityAIController->SetState(_currentState);
	ExecuteGraph();
}



//   E X E C U T I O N

void UOEntity::SetAIController(AEntityAIController* eaic) {
	_entityAIController = eaic;
}

void UOEntity::ExecuteGraph() {
	_entityAIController->SetNode(_brain.Peek());
	//_entityAIController->ExecuteNode();
}



// If a node can't be completed or is the last one, plot is considered completed
void UOEntity::NodeCompleted(bool completedOk) {
	//if (_mainPlotEntity && _currentState == State::idle) /*&& !_brain.Peek()->high_priority && !_brain.Peek()->nextNode->high_priority)*/
		//SetState(State::plot);
	if (completedOk && !_brain.IsLastNode()) {
		_brain.NextNode(); //BRANCH!!!
		ExecuteGraph();
	}
	else
	{
		if (_currentState == State::plot) {
			if (_mainPlotEntity == this) {
				for (UOEntity* e : _currentPlots[0]->GetInvolvedInPlot()) 
					e->SetMainPlotEntity(nullptr);
				_mainPlotEntity = nullptr;
				_currentPlots.erase(_currentPlots.begin());
				_plotGenerator->ChangeCurrentPlotsInAction(-1);
			}
			SetState(State::idle);
		}
		else if (_currentState == State::react) {
			if (_mainPlotEntity)
				SetState(State::plot);
			else
				SetState(State::idle);
		}
	}
}


void UOEntity::AddInstantNode(Node* n) {
	_brain.AddInstantNode(n);
//	_brain.NextNode();
}



//	 I N V E N T O R Y

vector<UOOwnable*> UOEntity::GetInventory() {
	return _inventory;
}
void UOEntity::StoreInInventory(UOOwnable* o) {
	_inventory.push_back(o);
}
bool UOEntity::RemoveFromInventory(UOOwnable* o) {
	int i = 0;
	for (UOOwnable* strd : _inventory) {
		if (o == strd) {
			RemoveFromInventory(i);
			return true;
		}
		i++;
	}
	return false;
		
}
bool UOEntity::RemoveFromInventory(int i) {
	if (i >= _inventory.size())
		return false;
	_inventory.erase(_inventory.begin() + i);
	return true;
}



// M E C H A N I C S

void UOEntity::Attack()
{
	if (!_isEntityAttacking) _isEntityAttacking = true;
}
bool UOEntity::GetIsEntityAttacking() {
	return _isEntityAttacking;
}
void UOEntity::SetIsEntityAttacking(bool attacking) {
	_isEntityAttacking = attacking;
}
void UOEntity::RebuildEdification(UOEdification * home)
{
	if (!_isEntityBuilding) _isEntityBuilding = true;
	//SET HOW MUCH
	home->RebuildEdification(5);
}
void UOEntity::StopRebuildEdification()
{
	_isEntityBuilding = false;
}

void UOEntity::GrabItem(UItem* item) {
	if (GetStrength() / 2.0f >= item->GetMass()) { // should this block be managed on UOEntity::GrabItem()?
		if (HasGrabbedItem()) {
			if (item->IsA<UOOwnable>())
				StoreInInventory((UOOwnable*)item);
			else
				ReleaseGrabbedItem();
		}
	}
	_grabbedItem = item;
	AActor* grabbedItemActor = _grabbedItem->GetOwner();
	grabbedItemActor->SetActorEnableCollision(false);
	AttachToSocket(grabbedItemActor, "RightHandSocket");
	grabbedItemActor->OnActorBeginOverlap.Add(HitFunc);
}

void UOEntity::ReleaseGrabbedItem() {
	AActor* grabbedItemActor = _grabbedItem->GetOwner();
	//dejar
	grabbedItemActor->OnActorBeginOverlap.Remove(HitFunc);
	grabbedItemActor->DetachRootComponentFromParent(true);
	grabbedItemActor->SetActorEnableCollision(true);
}

void UOEntity::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}

void UOEntity::AttachToSocket(AActor* target, string socket) {

	if (((ACharacter*)GetOwner())->GetMesh()->DoesSocketExist(socket.c_str()))
		target->AttachRootComponentTo(((ACharacter*)GetOwner())->GetMesh(), socket.c_str(), EAttachLocation::SnapToTarget, true);
	else GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("Trying to Attach to non-existing socket"));
}