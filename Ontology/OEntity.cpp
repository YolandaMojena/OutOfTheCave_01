// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "EntityAIController.h"
#include "Ontology/OEntity.h"
#include "Ontology/Ocivilian.h"
#include "BasePlot.h"

UOEntity::UOEntity() {
	_personality = new OPersonality();
}

UOEntity::UOEntity(OPersonality* personality) {
	_personality = personality;
}

void UOEntity::BeginPlay() {

	Super::BeginPlay();
	
	if (!IsPlayer) {
		for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
			plotGenerator = *Itr;
		//SetState(State::idle);
	}
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

bool UOEntity::GetIsDead() {
	return _isDead;
}

UOEntity* UOEntity::GetMainPlotEntity() {
	return _mainPlotEntity;
}

int UOEntity::GetStrength() {
	return _strength;
}

/*Graph* UOEntity::GetBrain() {
	return &_brain;
}*/



// R E L A T I O N S

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
		if (_relationships[i]->GetOtherEntity() == relative)
			break;
		else i++;
	}
	_relationships.erase(_relationships.begin() + i);
}

void UOEntity::DeletePossession(UOOwnable * possession)
{
	int i = 0;
	while (i < _possessions.size()) {
		if (_possessions[i]->GetOwnable() == possession)
			break;
		else i++;
	}
	_possessions.erase(_possessions.begin() + i);
}

void UOEntity::DeleteDesire(UOOwnable * desire)
{
	int i = 0;
	while (i < _materialDesires.size()) {

		if (_materialDesires[i]->GetOwnable() == desire)
			break;
		else i++;
	}
	_materialDesires.erase(_materialDesires.begin() + i);
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
void UOEntity::SendReport(Report * newReport)
{
	if (CheckValidPersonality(newReport->GetType())) {
		plotGenerator->AddReportToLog(newReport);
	}
}

void UOEntity::Die() {

	ACharacter* character = dynamic_cast<ACharacter*>(GetOwner());
	character->GetMesh()->SetSimulatePhysics(true);
	character->GetMesh()->AttachTo(character->GetCapsuleComponent());
	character->GetCapsuleComponent()->AttachTo(character->GetMesh());

	if (IsA<UOCivilian>())
		dynamic_cast<UOCivilian*>(this)->currentIconPath = "";

	_isDead = true;
}




// Generates notifications for reactiveness
// Generates Plot Reports and changes in ontology
// Deletes entity from the relationships lists of its relationships
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

			// CHANGE FOR HIGH APPRECIATION
			if (relationFromOther->GetAppreciation() >= 0/*relationFromOther->LOW_APPRECIATION*/) {
				ORelation* relationWithKiller = relationFromOther->GetEntity()->GetRelationWith(killer);

				if (!relationWithKiller) {
					relationFromOther->GetEntity()->AddRelationship(new ORelation(relationFromOther->GetEntity(), killer));
					relationWithKiller = relationFromOther->GetEntity()->GetRelationWith(killer);
				}

				relationWithKiller->ChangeAppreciation(-relationFromOther->GetAppreciation());

				//if (relationWithKiller->GetAppreciation() < relationWithKiller->LOW_APPRECIATION)
				relationFromOther->GetEntity()->SendReport(new Report(relationWithKiller, BasePlot::TypeOfPlot::aggressive, this));
			}
			o->GetOtherEntity()->DeleteRelation(this);
		}
	}
}

void UOEntity::SetMainPlotEntity(UOEntity* mpe) {
	_mainPlotEntity = mpe;
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


bool UOEntity::CheckValidPersonality(BasePlot::TypeOfPlot type) {

	switch (type) {

	case BasePlot::TypeOfPlot::aggressive:
	//	if (_personality->GetAggressiveness() < 50 || _personality->GetBraveness() < 50) return false;
		return true;

	case BasePlot::TypeOfPlot::possessive:
	//	if (_personality->GetMaterialist() < 50 || _personality->GetAggressiveness() < 50) return false;
		return true;

	case BasePlot::TypeOfPlot::resources: 
		return true;

	case BasePlot::TypeOfPlot::thankful:
		if (_personality->GetKindness() < 50 || _personality->GetSocial() < 50) return false;

	case BasePlot::TypeOfPlot::preventive:
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
		}
		else {
			if (_mainPlotEntity) {
				_brain = GetMainPlotEntity()->GetCurrentPlot()->GetGraph();

				Node* comeToEntity = new Node();
				comeToEntity->SetNodeType(NodeType::goToItem);
				//comeToEntity->SetPosition(_mainPlotEntity->GetOwner()->GetActorLocation());
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