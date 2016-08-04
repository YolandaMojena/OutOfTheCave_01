// Fill out your copyright notice in the Description page of Project Settings.
#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OEdification.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "EntityAIController.h"
#include "NarrativeGeneration/Ambition.h"
#include "Ontology/OEntity.h"
#include "Ontology/Ocivilian.h"
#include "BasePlot.h"

float UOEntity::MIN_INTEGRITY = 20.f;

UOEntity::UOEntity() {
	_personality = new OPersonality();
	_deadOwnable = CreateDefaultSubobject<UOOwnable>(TEXT("DeadOwnable"));

	_hands = NewObject<UOOwnable>();
	_hands->_name = _hands_name;
	_hands->_centerOfMass = _hands_centerOfMass;
	_hands->_edgeLength = _hands_edgeLength;
	_hands->_edgeSharpness = _hands_edgeSharpness;
	_hands->_funcDir = _hands_funcDir;
	_hands->_funcPos = _hands_funcPos;
	_hands->_grabDir = _hands_grabDir;
	_hands->_grabPos = _hands_grabPos;
	_hands->_mass = _hands_mass;
	_hands->_maxLength = _hands_maxLength;
	_hands->_spikes = _hands_spikes;
	_hands->_spiky = _hands_spiky;
	_hands->_toughness = _hands_toughness;
	_hands->_volume = _hands_volume;
}

UOEntity::UOEntity(OPersonality* personality) {
	_personality = personality;
	_deadOwnable = CreateDefaultSubobject<UOOwnable>(TEXT("DeadOwnable"));	
}

void UOEntity::BeginPlay() {

	Super::BeginPlay();

	if (!IsPlayer) {
		//((ACharacter*)GetOwner())->GetMesh()->SetAllBodiesBelowSimulatePhysics(((ACharacter*)GetOwner())->GetMesh()->GetBoneName(1), true);
		//for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
			//_plotGenerator = *Itr;
		
		GenerateTraits();
		HitFunc.BindUFunction(GetOwner(), "OnOverlapBegin");

		if(rand()% 100 < 50)
			_plotGenerator->AddNotorious(this);
	}
	else {
		_integrity = 10000000;
	}

	_skelMesh = ((ACharacter*)GetOwner())->GetMesh();
}

void UOEntity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_isEntityAttacking && _attackCooldown > 0) {
		_attackCooldown -= DeltaTime;
	}

	if (!IsPlayer) {
		for (TArray<const FAnimNotifyEvent*>::TIterator it = _skelMesh->AnimScriptInstance->AnimNotifies.CreateIterator(); it; ++it) {
			if ((*it)->NotifyName.ToString() == "EndAttack") EndAttack();
		}

		CleanKnownNotifyIDs(DeltaTime);
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
void UOEntity::ChangeNotoriety(int value) {
	_notoriety += value;
	_plotGenerator->AddNotorious(this);
}

bool UOEntity::GetIsNumb() {
	return _isNumb;
}

void UOEntity::SetIsNumb(bool value)
{
	_isNumb = value;
	if (value)
		SetState(State::numb);
	else
		RethinkState();
}

UOEntity* UOEntity::GetMainPlotEntity() {
	return _mainPlotEntity;
}

UItem* UOEntity::GetGrabbedItem() {
	if(_grabbedItem)
		return _grabbedItem;
	return _hands;
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
	if (IsPlayer) return "";
	else {
		FString raceName;
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERace"), true);
		if (EnumPtr) raceName = EnumPtr->GetEnumName((int32)_race);
		raceName.RemoveFromStart("R_");
		return raceName;
	}
}
float UOEntity::GetStrength() {
	return _strength;
}
float UOEntity::GetSpeed() {
	return _speed;
}
float UOEntity::GetAgility() {
	return _agility;
}
float UOEntity::GetKindness() {
	return _personality->GetKindness();
}
float UOEntity::GetAggressiveness() {
	return _personality->GetAggressiveness();
}
float UOEntity::GetBraveness() {
	return _personality->GetBraveness();
}
float UOEntity::GetAppreciationTo(UOEntity* ent) {
	return GetRelationWith(ent)->GetAppreciation();
}
float UOEntity::GetRespectTo(UOEntity* ent) {
	return GetRelationWith(ent)->GetRespect();
}
float UOEntity::GetFearTo(UOEntity* ent) {
	return GetRelationWith(ent)->GetFear();
}
EJob UOEntity::GetJob() {
	return _job;
}

AEntityAIController* UOEntity::GetEntityAIController() {
	return _entityAIController;
}


// S E T T E R S
void UOEntity::SetStrength(float st) {
	_strength = st;
}
void UOEntity::SetSpeed(float sd) {
	_speed = sd;
}
void UOEntity::SetAgility(float ag) {
	_agility = ag;
}
void UOEntity::SetRace(ERace race)
{
	_race = race;
}
void UOEntity::SetJob(EJob job) {
	_job = job;
}
void UOEntity::GenerateTraits() {
	const float BASE_MOVEMENT_SPEED = 300.f;
	const float MOVEMENT_SPEED_GROWTH = 6.f;
	const float BASE_JUMP_FORCE = 1400.f;
	const float JUMP_FORCE_GROWTH = 2.f;

	const int RANDOM_WIDTH = 5;
	//+ rand() % (RANDOM_WIDTH * 2) - RANDOM_WIDTH

	switch (_job) {
	case EJob::J_Farmer:
		SetStrength(20);
		SetSpeed(25);
		SetAgility(10);
		break;
	case EJob::J_Ironsmith:
		SetStrength(30);
		SetSpeed(15);
		SetAgility(10);
		break;
	case EJob::J_Miner:
		SetStrength(30);
		SetSpeed(15);
		SetAgility(15);
		break;
	case EJob::J_Peasant:
		SetStrength(20);
		SetSpeed(20);
		SetAgility(10);
		break;
	case EJob::J_Shaman:
		SetStrength(10);
		SetSpeed(10);
		SetAgility(10);
		break;
	case EJob::J_Soldier:
		SetStrength(30);
		SetSpeed(30);
		SetAgility(30);
		break;
	
	case EJob::J_Herbibore:
		SetStrength(50);
		SetSpeed(50);
		SetAgility(50);
		break;
	case EJob::J_Predator:
		SetStrength(60);
		SetSpeed(60);
		SetAgility(60);
		break;
	}

	SetStrength(GetStrength() + rand() % (RANDOM_WIDTH * 2) - RANDOM_WIDTH);
	SetSpeed(GetSpeed() + rand() % (RANDOM_WIDTH * 2) - RANDOM_WIDTH);
	SetAgility(GetAgility() + rand() % (RANDOM_WIDTH * 2) - RANDOM_WIDTH);

	((ACharacter*)GetOwner())->GetCharacterMovement()->MaxWalkSpeed = BASE_MOVEMENT_SPEED + MOVEMENT_SPEED_GROWTH * GetSpeed();
	((ACharacter*)GetOwner())->GetCharacterMovement()->JumpZVelocity = BASE_JUMP_FORCE + JUMP_FORCE_GROWTH * (GetStrength() + GetAgility());
}

// R E L A T I O N S

void UOEntity::AddRelationship(ORelation* newRelation) {
	if (newRelation->GetOtherEntity() != newRelation->GetEntity() && newRelation->GetEntity() == this) {
		_relationships.push_back(newRelation);
		DeletePotentitalRelationship(newRelation->GetOtherEntity());
	}
}
void UOEntity::AddRelationship(UOEntity* newEntity) {
	if (newEntity != this) {
		_relationships.push_back(new ORelation(this, newEntity, _personality->GetSocial(), newEntity->GetNotoriety(), _personality->GetMaxValue() - _personality->GetBraveness()));
		DeletePotentitalRelationship(newEntity);
	}
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




// E V E N T S


void UOEntity::ReceiveDamage(float damage, UOEntity * damager)
{
	if (!_isNumb && !IsPlayer) {

		_integrity -= damage;
		_attacker = damager;

		CastNotify((UItem*) this, damager, ENotify::N_Damaged);

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

	_isNumb = true;

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
*/
	
	// Handle plot state and main entity

	if (_currentState == State::plot) {
		if (_mainPlotEntity == this) {
			if (_currentPlots.size() > 0 && !_currentPlots[0]->GetIsExclusive() && _currentPlots[0]->GetInvolvedInPlot().Num() > 0) {

				UOEntity* notorious = _currentPlots[0]->GetInvolvedInPlot()[0];
				for (UOEntity* entity : _currentPlots[0]->GetInvolvedInPlot()) {
					if (entity->GetNotoriety() >= notorious->GetNotoriety())
						notorious = entity;
				}
				for (UOEntity* entity : _currentPlots[0]->GetInvolvedInPlot()) {
					entity->SetMainPlotEntity(notorious);
				}
			}
			_plotGenerator->ChangeCurrentPlotsInAction(-1);
		}
	}

	_plotGenerator->DeleteNotorious(this);
	SetState(State::numb);
	//GetOwner()->RemoveOwnedComponent(this);
	//this->DestroyComponent();
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

	/*for (FHitResult hr : outHits) {
		UOEntity* entity = hr.GetActor()->FindComponentByClass<UOEntity>();
		//if (entity->IsInSight(GetOwner())) {
			// Generate Notification
		}	
	}*/

	//   P L O T S
	//	Search in relationships for those who appreciate entity, change ontology if required and send reports

	vector<ORelation*> relations = GetRelationships();

	for (ORelation* o : relations) {
		ORelation* relationFromOther = o->GetOtherEntity()->GetRelationWith(this);

		if (relationFromOther){
			// CHANGE FOR HIGH APPRECIATION
			if (relationFromOther->GetAppreciation() >= relationFromOther->LOW_APPRECIATION) {
				ORelation* relationWithKiller = relationFromOther->GetEntity()->GetRelationWith(killer);

				if (!relationWithKiller) {
					relationFromOther->GetEntity()->AddRelationship(new ORelation(relationFromOther->GetEntity(), killer));
					relationWithKiller = relationFromOther->GetEntity()->GetRelationWith(killer);
				}

				relationWithKiller->ChangeAppreciation(-relationFromOther->GetAppreciation());
				if (relationWithKiller->GetAppreciation() < relationWithKiller->LOW_APPRECIATION)
				relationFromOther->GetEntity()->SendReport(new Report(relationWithKiller, TypeOfPlot::aggressive, this));
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
	if(_currentPlots.size()>0)
		return _currentPlots[0];
	else return nullptr;
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
	//	if (_personality->GetMaterialist() < 50 || _personality->GetBraveness() < 50) return false;
		return true;

	// Every entity worries about it's home and basic needs 
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

Graph * UOEntity::GetIdleGraph()
{
	return _idleGraph;
}

Graph * UOEntity::GetBrain()
{
	return &_brain;
}

UOEntity::State UOEntity::GetCurrentState() {
	return _currentState;
}

void UOEntity::SetState(State s) {

	if (_currentState != s || !_brain.Peek()) {  //HACK/MOCK/CHAPUZA
		_currentState = s;

		switch (_currentState) {
		case State::idle:
		{
			_brain = *_idleGraph;
			if (_brain.Peek()->nBlackboard.daytime >= 0) {
				while (_brain.Peek()->nBlackboard.daytime < _plotGenerator->dayTime) {
					if (_brain.IsLastNode()) {
						_brain = *_idleGraph;
						break;
					}
					else
						_brain.NextNode();
				}
			}
		}
		break;

		case State::plot:
		{
			if (_currentPlots.size() > 0 && !_mainPlotEntity) {
				_brain = _currentPlots[0]->GetGraph();
				_mainPlotEntity = this;
				_plotGenerator->ChangeCurrentPlotsInAction(+1);
				_currentPlots[0]->SavePlotToFile(Utilities::SavePath, Utilities::PlotFile);
				_currentPlots[0]->PrintSentence();
			}
			else {
				if (_mainPlotEntity && _mainPlotEntity->GetCurrentPlot()) {

					_brain = _mainPlotEntity->GetCurrentPlot()->GetGraph();

					Node* comeToEntity = new Node();
					comeToEntity->SetNodeType(NodeType::goToActor);
					comeToEntity->SetActor(_mainPlotEntity->GetOwner());
					_brain.AddInstantNode(comeToEntity);
					_brain.NextNode();
				}
			}
		}
			break;

		case State::react:
		{
			_brain = *_currentReacts[0]; //*g
		}
			break;

		case State::numb:
		{
			_isNumb = true;
			_brain = NULL;
			if (IsA<UOCivilian>()) {
				((UOCivilian*)this)->currentIconPath = "";
				FText TestHUDText = NSLOCTEXT("FText Namespace", "Key", "");
				GetOwner()->FindComponentByClass<UTextRenderComponent>()->Text = TestHUDText;
			}	
		}
			break;

		default:
			_brain = *_idleGraph;
			break;
		}

		_entityAIController->SetState(_currentState);
	}
	
	if(_currentState != State::numb) ExecuteGraph();
}

//   N O T I F Y

void UOEntity::ReceiveNotify(UItem* predicate, UOEntity* subject, ENotify notifyType, FString notifyID) {
	if (IsPlayer)
		return;

	for (FString id : _knownNotifyIDs) {
		if (id == notifyID)
			return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GetOwner()->GetName() + TEXT(" received a Notify! ") + notifyID);

	_knownNotifyIDs.push_back(notifyID);
	
	//Check relation with predicate
	bool doICare = false;
	ORelation* relation = GetRelationWith((UOEntity*) predicate);
	OOwnership* ownership = GetOwnershipWith((UOOwnable*) predicate);
	if (relation && (relation->GetAppreciation() > 50 || relation->GetRespect() > 75)) {
		doICare = true;
	}
	else if (ownership && ownership->GetWorth() > 50) {
		doICare = true;
	}

	//Check relation with subject
	if (doICare) {
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GetOwner()->GetName() + TEXT(" thinks this compels him!"));
		Graph* reactGraph = new Graph();
		Node* n;
		ORelation* relationWithSubject = GetRelationWith(subject);
		if (!relationWithSubject || (relationWithSubject->GetAppreciation() < 50 && relationWithSubject->GetRespect() < 75)) {
			//Se va a liar parda
			n = new Node();
			n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon);
			reactGraph->AddNode(n);
			n = new Node();
			n->SetNodeType(NodeType::goToActor);  n->SetActor(subject->GetOwner());
			reactGraph->AddNode(n);
			n = new Node();
			n->SetNodeType(NodeType::attack);  n->SetEntity(subject); n->SetActor(subject->GetOwner());
			reactGraph->AddNode(n);

			_currentReacts.push_back(reactGraph);

			GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GetOwner()->GetName() + TEXT(" wants to smash ") + subject->GetOwner()->GetName() + TEXT("!"));

			RethinkState();
		}
		else if(relation) { //(make peace only when we are dealing between two entities fighting. Otherwise we're not doing anything.
			// Peace and love
			//RethinkState();

			n = new Node();
			n->SetNodeType(NodeType::goToActor);  n->SetActor(subject->GetOwner());
			reactGraph->AddNode(n);
			n = new Node();
			n->SetNodeType(NodeType::stopFight); n->SetActor(subject->GetOwner());
			reactGraph->AddNode(n);

			n = new Node();
			n->SetNodeType(NodeType::goToActor);  n->SetActor(predicate->GetOwner());
			reactGraph->AddNode(n);
			n = new Node();
			n->SetNodeType(NodeType::stopFight); n->SetActor(predicate->GetOwner());
			reactGraph->AddNode(n);

			GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GetOwner()->GetName() + TEXT(" wants to stop the fight!"));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GetOwner()->GetName() + TEXT(" can't act!"));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GetOwner()->GetName() + TEXT(" doesn't give a fuck"));
	}
}

void UOEntity::CleanKnownNotifyIDs(float deltaTime) {
	const int NOTIFY_LIFESPAN = 3;
	if (!_knownNotifyIDs.empty()) {
		if (_notifyDeadline >= NOTIFY_LIFESPAN) {
			_notifyDeadline = 0.f;
			_knownNotifyIDs.erase(_knownNotifyIDs.begin());
		}
		else
			_notifyDeadline += deltaTime;
	}
}
//   E X E C U T I O N

void UOEntity::SetAIController(AEntityAIController* eaic) {
	_entityAIController = eaic;
}

void UOEntity::SetPlotGenerator()
{
	for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
		_plotGenerator = *Itr;
}

void UOEntity::ExecuteGraph() {
	_entityAIController->SetNode(_brain.Peek());
	//_entityAIController->ExecuteNode();
}



// If a node can't be completed or is the last one, plot is considered completed
void UOEntity::NodeCompleted(bool completedOk) {
	if (completedOk && !_brain.IsLastNode()) {
		_brain.NextNode();
		if (_currentState == State::plot){
			//_currentPlots[0]->GetGraphPointer()->NextNode();
		}
			
		else if (_currentState == State::react)
			_currentReacts[0]->NextNode();
	}
	else {
		ClearState(completedOk);
	}

	_isEntityAttacking = false;
	_isEntityCultivating = false;
	_isEntityMining = false;
	_isEntityBuilding = false;

	RethinkState();
}

void UOEntity::ClearState(bool completedOk)
{
 	if (_currentState == State::plot) {
		if (_mainPlotEntity == this) {
			//if (!completedOk)
			if (GetCurrentPlot()) {

				for (UOEntity* e : _currentPlots[0]->GetInvolvedInPlot()) {
					if (e->GetMainPlotEntity() == this) {
						e->SetMainPlotEntity(nullptr);
						e->RethinkState();
					}
				}

				_mainPlotEntity = nullptr;
				_currentPlots.erase(_currentPlots.begin());
				_plotGenerator->ChangeCurrentPlotsInAction(-1);
			}
		}
		else {
			SetMainPlotEntity(nullptr);
		}
	}
	else if (_currentState == State::react) {
		_currentReacts.erase(_currentReacts.begin());
	}

	_currentState = State::restart;
}

/*void UOEntity::AddInstantNode(Node* n) {
	_brain.AddInstantNode(n);
}*/
void UOEntity::AddInstantHelpNode(Node * n)
{
	_brain.AddInstantNode(n);
}

void UOEntity::RethinkState() {	
	if (!_isNumb) {
		// Current action is of high priority
		if (_brain.Peek() && _brain.Peek()->nBlackboard.isHighPriority) {
			SetState(_currentState);
		}

		// Hay un react en high priority
		/*else if _currentReacts[0] && _currentReacts[0]->Peek()->nBlackboard.isHighPriority)
			SetState(State::react);*/

		// Hay un react
		else if (!_currentReacts.empty()) {
			GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GetOwner()->GetName() + TEXT(" wants to react!"));
			SetState(State::react);
		}

			// Pending plot to execute
		else if (!_currentPlots.empty() || _mainPlotEntity)
			SetState(State::plot);

		// Nothing important to do, continue to idle
		else
			SetState(State::idle);
	}
}

//	 I N V E N T O R Y

vector<UOOwnable*> UOEntity::GetInventory() {
	return _inventory;
}
void UOEntity::StoreInInventory(UOOwnable* o) {
	_inventory.push_back(o);

	o->UnregisterComponent();

	/*********************/
	o->GetOwner()->Destroy();
	/********************/

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow, TEXT("Item Stored!"));
	for (UOOwnable* stored : _inventory) {
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow, stored->GetItemName());
	}

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

void UOEntity::GrabFromInventory(UOOwnable* o) {
	SpawnFromInventory(o);
	GrabItem((UItem*)o);
}

void UOEntity::SpawnFromInventory(UOOwnable* o) {
	int i = 0;
	for (UOOwnable* strd : _inventory) {
		if (o == strd) {
			AOwnableSpawner os;
			os.SpawnOwnable(o, (UItem*)this);
			RemoveFromInventory(i);
			return;
		}
		i++;
	}
}
void UOEntity::SpawnFromInventory(int i) {
	if (i < _inventory.size()) {
		//AActor* itemToSpawn = GetWorld()->SpawnActor<AActor>(BP_Bear, GetOwner()->GetActorLocation() + RandomDisplacement(1000), GetActorRotation(), SpawnParams);
	}
}

// M E C H A N I C S

void UOEntity::Attack()
{
	//const float BASE_ATTACK_COOLDOWN = 1.2f;

	if (!_isEntityAttacking) {
		_isEntityAttacking = true;
		//_attackCooldown = BASE_ATTACK_COOLDOWN - _agility / 100.f;
	}
}
void UOEntity::DamageEdification(UOEdification * targetEdification)
{
	//ERIC'S AFFORDANCES
	targetEdification->ReceiveDamage(_strength, this);

	if (targetEdification->edificationType == EdificationType::house) {
		targetEdification->GetOwner()->FindComponentByClass<UDestructibleComponent>()->ApplyRadiusDamage(_strength, targetEdification->GetCenterOfMass(), 32, 0.01, false);
	}
}

void UOEntity::EndAttack() {
	const float BASE_ATTACK_COOLDOWN = 1.2f;

	_isEntityAttacking = false;
	_attackCooldown = BASE_ATTACK_COOLDOWN - _agility / 100.f;
}
bool UOEntity::IsEntityAttacking() {
	return _isEntityAttacking;
}
float UOEntity::GetAttackCooldown() {
	return _attackCooldown;
}
/*void UOEntity::SetIsEntityAttacking(bool attacking) {
	_isEntityAttacking = attacking;
}*/
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
			if (_grabbedItem && _grabbedItem->IsA<UOOwnable>())
				StoreInInventory((UOOwnable*)_grabbedItem);
			else
				ReleaseGrabbedItem();
		}

		_grabbedItem = item;
		AActor* grabbedItemActor = _grabbedItem->GetOwner();
		grabbedItemActor->SetActorEnableCollision(false);
		AttachToSocket(grabbedItemActor, "RightHandSocket");
		grabbedItemActor->OnActorBeginOverlap.Add(HitFunc);
		((UOOwnable*)_grabbedItem)->SetIsGrabbed(true);
	}
}

void UOEntity::ReleaseGrabbedItem() {
	if (_grabbedItem) {
		AActor* grabbedItemActor = _grabbedItem->GetOwner();
		//dejar
		grabbedItemActor->OnActorBeginOverlap.Remove(HitFunc);
		grabbedItemActor->DetachRootComponentFromParent(true);
		grabbedItemActor->SetActorEnableCollision(true);
		((UOOwnable*)_grabbedItem)->SetIsGrabbed(false);
		_grabbedItem = nullptr;
	}
}


bool UOEntity::StealFromInventory(UOOwnable * o, UOEntity * buggler)
{
	int i = 0;
	for (UOOwnable* strd : _inventory) {
		if (o == strd) {
			buggler->StoreInInventory(o);
			o->AddOwner(buggler);
			RemoveFromInventory(i);
			o->IHaveBeenStolenBySomeone(this, buggler);
			return true;
		}
		i++;
	}
	return false;
}

void UOEntity::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}

void UOEntity::AttachToSocket(AActor* target, string socket) {

	if (((ACharacter*)GetOwner())->GetMesh()->DoesSocketExist(socket.c_str()))
		target->AttachRootComponentTo(((ACharacter*)GetOwner())->GetMesh(), socket.c_str(), EAttachLocation::SnapToTarget, true);
	else GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("Trying to Attach to non-existing socket"));
}
