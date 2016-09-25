// Fill out your copyright notice in the Description page of Project Settings.
#include "OutOfTheCave_01.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OEdification.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OntologicFunctions.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "EntityAIController.h"
#include "NarrativeGeneration/Ambition.h"
#include "Ontology/OEntity.h"
#include "Ontology/Ocivilian.h"
#include "FNearbyEntitiesFinder.h"
#include "BasePlot.h"
#include "Ontology/ThreadManager.h"
#include "Components/WidgetComponent.h"


float UOEntity::MIN_INTEGRITY = 20.f;

UOEntity::UOEntity() {
	//if(!IsPlayer) GenerateTraits();  -> Called on spawning (either from Residence or from WorldPlot)
}

/*UOEntity::UOEntity(OPersonality* personality) {
	_personality = personality;
	_deadOwnable = CreateDefaultSubobject<UOOwnable>(TEXT("DeadOwnable"));	
}*/

void UOEntity::BeginPlay() {

	Super::BeginPlay();

	if (IsPlayer) {
		GenerateTraits(); //->for other entities, called on spawning (either from Residence or from WorldPlot)
		SetRace(ERace::R_Troll);
	}
	else{
		HitFunc.BindUFunction(GetOwner(), "OnOverlapBegin");
	}

	_skelMesh = ((ACharacter*)GetOwner())->GetMesh();
}


void UOEntity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (IsDead)
		return;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (_isEntityAttacking && _attackCooldown > 0) {
		_attackCooldown -= DeltaTime;
	}*/

	if (IsInitialized && !_isRecognized) {
		_isRecognized = true;
		_plotGenerator->allEntities.Add(this);
	}

	if (!IsPlayer && IsInitialized) {
		for (TArray<const FAnimNotifyEvent*>::TIterator it = _skelMesh->AnimScriptInstance->AnimNotifies.CreateIterator(); it; ++it) {
			if ((*it)->NotifyName.ToString() == "EndAttack") EndAttack();
		}

		CleanKnownNotifyIDs(DeltaTime);

		if (!_searchingNearbyEntities && _currentState != AIState::numb) {
			if (entitiesFinderDelay < ENTITIES_FINDER_DELAY) {
				entitiesFinderDelay += DeltaTime;
			}
			else {
				_searchingNearbyEntities = true;
				entitiesFinderDelay = 0.f;
				ThreadManager::AddRequest(this);
				//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::FromHex("#FF8811"), GetItemName() + TEXT(" STARTED searching for nearby entities."));
			}
		}		
	}
}


// G E T T E R S

UOOwnable* UOEntity::GetHands() {
	UOOwnable* hands = NewObject<UOOwnable>();
	hands->_name = _hands_name;
	hands->_mass = _hands_mass;
	hands->_volume = _hands_volume;
	hands->_toughness = _hands_toughness;
	hands->_edgeSharpness = _hands_edgeSharpness;
	hands->_edgeLength = _hands_edgeLength;
	hands->_spiky = _hands_spiky;
	hands->_nbrSpikes = _hands_nbrSpikes;
	hands->_elongation = _hands_elongation;
	hands->_r_maxLength = _hands_r_maxLength;
	hands->_r_grabPos = _hands_r_grabPos;
	hands->_r_grabDir = _hands_r_grabDir;
	hands->_r_funcPos = _hands_r_funcPos;
	hands->_r_funcDir = _hands_r_funcDir;
	hands->_r_funcPlane = _hands_r_funcPlane;
	hands->_r_centerOfMass = _hands_r_centerOfMass;
	return hands;
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
void UOEntity::ChangeNotoriety(int value) {
	_notoriety += value;
	_plotGenerator->AddNotorious(this);
}

bool UOEntity::GetIsNumb() {
	return GetCurrentState() == AIState::numb;
}

UOEntity* UOEntity::GetMainPlotEntity() {
	return _mainPlotEntity;
}

UItem* UOEntity::GetGrabbedItem() {
	if (_grabbedItem) {
		return _grabbedItem;
	}
	return GetHands();
}
bool UOEntity::HasGrabbedItem() {
	return (_grabbedItem && _grabbedItem != nullptr && _grabbedItem != NULL && _grabbedItem->IsValidItem());
}


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
float UOEntity::GetBraveness() {
	return _personality->GetBraveness();
}
float UOEntity::GetAggressiveness() {
	return _personality->GetAggressiveness();
}
float UOEntity::GetSocial() {
	return _personality->GetSocial();
}
float UOEntity::GetMaterialist () {
	return _personality->GetMaterialist();
}
float UOEntity::GetCurious() {
	return _personality->GetCurious();
}
float UOEntity::GetAstute() {
	return _personality->GetAstute();
}
float UOEntity::GetPride() {
	return _personality->GetPride();
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
FVector UOEntity::GetGoingLocation() {

	AActor* targetActor = _brain.Peek()->nBlackboard.actor;
	if (targetActor)
		return targetActor->GetActorLocation();

	UOEntity* targetEntity = _brain.Peek()->nBlackboard.entity;
	if (targetEntity)
		return targetEntity->GetOwner()->GetActorLocation();

	UOEdification* targetEdification = _brain.Peek()->nBlackboard.edification;
	if (targetEdification)
		return targetEdification->GetOwner()->GetActorLocation();

	UOOwnable* targetOwnable = _brain.Peek()->nBlackboard.ownable;
	if (targetOwnable)
		return targetOwnable->GetOwner()->GetActorLocation();

	FVector position = _brain.Peek()->nBlackboard.position;
	if (position.Z != -1)
		return position;

	return GetHome()->GetOwner()->GetActorLocation();
}

float UOEntity::GetAppreciationToOtherRace(ERace race)
{
	int raceCounter = 0;
	int appreciationSum = 0;
	int lowestAppreciation = 100;

	for (UOEntity* o : _plotGenerator->allEntities){
		if (!o || !o->IsValidItem())
			continue;

		if (o->GetRace() == race) {
			raceCounter++;
			ORelation* relation = GetRelationWith(o);
			if (relation)  {
				appreciationSum += relation->GetAppreciation();
				if (relation->GetAppreciation() < lowestAppreciation)
					_mostHatedEntity = o;
			}		
		}
	}
	if (raceCounter == 0) return 100;
	return appreciationSum / raceCounter;
}

UOEntity* UOEntity::GetMostHated() {
	GetAppreciationToOtherRace(_race == ERace::R_Human ? ERace::R_Goblin : ERace::R_Human);
	return _mostHatedEntity;
}

AEntityAIController* UOEntity::GetEntityAIController() {
	return _entityAIController;
}

TArray<UOEntity*> UOEntity::GetNearbyEntities() {
	return _nearbyEntities;
}
TArray<UOEntity*>* UOEntity::GetNearbyEntitiesPtr() {
	return &_nearbyEntities;
}
TArray<UOEntity*>* UOEntity::GetEntitiesReactingToPtr() {
	return &_entitiesReactingTo;
}

UOEdification* UOEntity::GetHome() {
	return _entityHome;
}


// S E T T E R S
void UOEntity::SetStrength(float st) {
	if (st > 100)
		_strength = 100;
	else if (st < 0)
		_strength = 0;
	else
		_strength = st;
}
void UOEntity::SetSpeed(float sp) {
	if (sp > 100)
		_speed = 100;
	else if (sp < 0)
		_speed = 0;
	else
		_speed = sp;
}
void UOEntity::SetAgility(float ag) {
	if (ag > 100)
		_agility = 100;
	else if (ag < 0)
		_agility = 0;
	else
		_agility = ag;
}
void UOEntity::ChangeStrength(float add) {
	SetStrength(GetStrength() + add);
}
void UOEntity::ChangeSpeed(float add) {
	SetSpeed(GetSpeed() + add);
}
void UOEntity::ChangeAgility(float add) {
	SetAgility(GetAgility() + add);
}
void UOEntity::SetRace(ERace race)
{
	_race = race;
}
void UOEntity::SetJob(EJob job) {
	_job = job;
}


// G E N E R A T E    T R A I T S   -   INITIALIZE ENTITY
void UOEntity::GenerateTraits() {

	IsInitialized = true;

	
	// R A C E - D E F I N E D   T R A I T S
	int PERSONALITY_SPARE = 25;

	switch (_race) {
	case ERace::R_Human:
		_personality = new OPersonality(
			40 + rand() % PERSONALITY_SPARE,
			10 + rand() % (PERSONALITY_SPARE * 3),
			40 + rand() % PERSONALITY_SPARE,
			10 + rand() % (PERSONALITY_SPARE * 3),
			10 + rand() % (PERSONALITY_SPARE * 3),
			10 + rand() % (PERSONALITY_SPARE * 3),
			10 + rand() % PERSONALITY_SPARE,
			75 + rand() % PERSONALITY_SPARE
			);
		SetStrength(20);
		SetSpeed(15);
		SetAgility(10);
		break;
	case ERace::R_Goblin:
		_personality = new OPersonality(
			75 + rand() % PERSONALITY_SPARE,
			75 + rand() % PERSONALITY_SPARE,
			75 + rand() % PERSONALITY_SPARE,
			75 + rand() % PERSONALITY_SPARE,
			75 + rand() % PERSONALITY_SPARE,
			75 + rand() % PERSONALITY_SPARE,
			75 + rand() % PERSONALITY_SPARE,
			15 + rand() % PERSONALITY_SPARE
			);
		SetStrength(15);
		SetSpeed(20);
		SetAgility(20);
		break;
	case ERace::R_Bear:
		_personality = new OPersonality(
			0,
			100,
			100,
			0,
			0,
			75,
			75,
			0
			);
		SetStrength(80);
		SetSpeed(30);
		SetAgility(60);
		break;
	case ERace::R_Herbivore:
		_personality = new OPersonality(
			75,
			10,
			25,
			100,
			0,
			50,
			25,
			0
			);
		SetStrength(20);
		SetSpeed(50);
		SetAgility(50);
		break;
	case ERace::R_Wolf:
		_personality = new OPersonality(
			50,
			75,
			100,
			100,
			0,
			50,
			75,
			0
			);
		SetStrength(60);
		SetSpeed(60);
		SetAgility(60);
		break;
	case ERace::R_Troll:
		_personality = new OPersonality(
			50,
			50,
			50,
			50,
			50,
			50,
			50,
			50
			);
		break;
	default:
		_personality = new OPersonality();
		break;
	}

	// J O B - D E F I N E D   T R A I T S
	if (!IsPlayer) {

		const float BASE_MOVEMENT_SPEED = 300.f;
		const float MOVEMENT_SPEED_GROWTH = 6.f;
		const float BASE_JUMP_FORCE = 1400.f;
		const float JUMP_FORCE_GROWTH = 2.f;

		const int RANDOM_WIDTH = 5;
		//+ rand() % (RANDOM_WIDTH * 2) - RANDOM_WIDTH

		switch (_job) {
		case EJob::J_Farmer:
			break;
		case EJob::J_Ironsmith:
			break;
		case EJob::J_Miner:
			ChangeStrength(10);
			//SetSpeed(15);
			//SetAgility(15);
			break;
		case EJob::J_Peasant:
			ChangeStrength(5);
			//ChangeSpeed(5);
			//ChangeAgility(5);
			break;
		case EJob::J_Shaman:
			//SetStrength(10);
			//SetSpeed(10);
			//SetAgility(10);
			break;
		case EJob::J_Soldier:
			ChangeStrength(15);
			ChangeSpeed(15);
			ChangeAgility(15);
			break;

		case EJob::J_Herbibore:
			//SetStrength(50);
			//SetSpeed(50);
			//SetAgility(50);
			break;
		case EJob::J_Predator:
			//ChangeStrength(10);
			//ChangeSpeed(10);
			//ChangeAgility(10);
			break;
		}

		ChangeStrength(rand() % (RANDOM_WIDTH * 2) - RANDOM_WIDTH);
		ChangeSpeed(rand() % (RANDOM_WIDTH * 2) - RANDOM_WIDTH);
		ChangeAgility(rand() % (RANDOM_WIDTH * 2) - RANDOM_WIDTH);

		_attackCooldown = 1.5f - GetAgility() / 100.f;

		((ACharacter*)GetOwner())->GetCharacterMovement()->MaxWalkSpeed = BASE_MOVEMENT_SPEED + MOVEMENT_SPEED_GROWTH * GetSpeed() * 2;
		((ACharacter*)GetOwner())->GetCharacterMovement()->JumpZVelocity = BASE_JUMP_FORCE + JUMP_FORCE_GROWTH * (GetStrength() + GetAgility());

	}

	TraitsString = FString(
		"Strength: " + FString::SanitizeFloat(_strength) + "<br>" +
		"Agility: " + FString::SanitizeFloat(_agility) + "<br>" +
		"Speed: " + FString::SanitizeFloat(_speed) + "<br>" + "<br>" +
		"Kindness: " + FString::SanitizeFloat(GetKindness()) + "<br>" +
		"Braveness: " + FString::SanitizeFloat(GetBraveness()) + "<br>" +
		"Aggressiveness: " + FString::SanitizeFloat(GetAggressiveness()) + "<br>" +
		"Social: " + FString::SanitizeFloat(GetSocial()) + "<br>" +
		"Materialist: " + FString::SanitizeFloat(GetMaterialist()) + "<br>" +
		"Astute: " + FString::SanitizeFloat(GetAstute()) + "<br>" +
		"Proud: " + FString::SanitizeFloat(GetPride()) + "<br>"
		);
}

void UOEntity::SetHome(UOEdification* newHome) {
	_entityHome = newHome;
	if(_entityHome && _entityHome->IsValidItem())
		AddPossession(new OOwnership(this, ((UOOwnable*)_entityHome), 25 + GetMaterialist()));
}

// R E L A T I O N S

void UOEntity::AddRelationship(ORelation* newRelation) {
	if (!newRelation || !newRelation->GetEntity() || !newRelation->GetEntity()->IsValidItem() || !newRelation->GetOtherEntity() || !newRelation->GetOtherEntity()->IsValidItem())
		return;

	if (newRelation->GetOtherEntity() != newRelation->GetEntity() && newRelation->GetEntity() == this) {
		_relationships.push_back(newRelation);
	}
}
ORelation* UOEntity::AddRelationship(UOEntity* newEntity) {
	if (IsValidItem() && newEntity && newEntity->IsValidItem() && newEntity != this) {
		//ORelation* newRelation = new ORelation(this, newEntity, _personality->GetSocial(), newEntity->GetNotoriety(), _personality->GetMaxValue() - _personality->GetBraveness());
		int newAppreciation = 25 + GetSocial()*3/4;
		if (GetHome() && newEntity->GetHome()) {
			if(GetHome()->villageID == newEntity->GetHome()->villageID) {
				if(GetHome()->villageID != 0)
					newAppreciation += rand() % 20;
				newAppreciation += GetHome()->edificationID == newEntity->GetHome()->edificationID ? 20 : 0;
			}
		}
		if (GetRace() != newEntity->GetRace()) {
			newAppreciation -= (10 + rand() % 10);
		}
		ORelation* newRelation = new ORelation(this, newEntity,
			newAppreciation,	// Appreciation,
			newEntity->GetNotoriety(),																		// Respect
			newEntity->GetStrength() - GetStrength() + newEntity->GetAggressiveness()/3.f - GetBraveness()/3.f);	// Fear
		_relationships.push_back(newRelation);
		return newRelation;
	}
	return nullptr;
	//return NULL;
}

void UOEntity::AddPossession(OOwnership* newPossession) {
	if (!newPossession)
		return;

	_possessions.push_back(newPossession);
	newPossession->GetOwnable()->AddOwner(this);
}
void UOEntity::AddPossession(UOOwnable* newOwnable) {
	if (!newOwnable || !newOwnable->IsValidItem())
		return;

	_possessions.push_back(new OOwnership(this, newOwnable, _personality->GetMaterialist() * (1 + 10*newOwnable->GetRarityAsInt()/100.f)));
	newOwnable->AddOwner(this);
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
	if (GetCurrentState() != AIState::numb && !IsPlayer) {

		_integrity -= damage;
		_attacker = damager;

		

		if (_integrity < MIN_INTEGRITY) {
			IHaveBeenKilledBySomeone(damager); // <- CastNotify((UItem*) this, killer, ENotify::N_Obliterated);
			Die();
		}
		else {
			CastNotify((UItem*) this, damager, ENotify::N_Damaged);

			ORelation* r = GetRelationWith(damager);
			if (!r)
				r = AddRelationship(damager);
			if (r) {
				r->ChangeAppreciation(-damage);
				r->ChangeFear(damage / GetBraveness());

				if (r->GetFear() > GetBraveness()) {
					Graph* react = new Graph();
					Node* n = new Node();
					n->SetNodeType(NodeType::flee); n->SetActor(damager->GetOwner()); n->SetHighPriority();
					react->AddNode(n);
					AddReactGraph(react, damager);
				}
				else if (r->GetAppreciation() < GetAggressiveness() + GetPride()) {
					Graph* react = new Graph();
					Node* n = new Node();
					n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetHighPriority();
					react->AddNode(n);
					n = new Node();
					n->SetNodeType(NodeType::attack); n->SetEntity(damager); n->SetHighPriority();
					react->AddNode(n);
					AddReactGraph(react, damager);
				}
			}
		}
	}
}

// BEFORE SENDING, ELEGIBLE TYPE MUST BE CHECKED WITH PERSONALITY
void UOEntity::SendReport(Report* newReport)
{
	if (GetHome()->IsValidItem() && CheckValidPersonality(newReport->GetType())) {
		_plotGenerator->AddReportToLog(newReport);
	}
}

// When an entity dies he turns into an ownable
// Those who appreciate the entity obtain the possession of that ownable where worth = appreciation and lose the relationship
// If the entity is involved in a plot, the most notorious entity ascends to main entity
void UOEntity::Die() {

	IsDead = true;

	ACharacter* character = (ACharacter*)GetOwner();
	character->GetMesh()->SetSimulatePhysics(true);
	character->GetMesh()->AttachTo(character->GetCapsuleComponent());
	character->GetCapsuleComponent()->AttachTo(character->GetMesh());

	if (IsA<UOCivilian>()) {

		((UOCivilian*)this)->currentIconPath = "";

		UWidgetComponent* iconComp = GetOwner()->FindComponentByClass<UWidgetComponent>();
		if (iconComp)
			iconComp->SetVisibility(false, true);

		FText TestHUDText = NSLOCTEXT("FText Namespace", "Key", "");
		GetOwner()->FindComponentByClass<UTextRenderComponent>()->Text = TestHUDText;
		GetOwner()->FindComponentByClass<UTextRenderComponent>()->SetVisibility(false);

		ReleaseGrabbedItem();
	}

	if (_currentState == AIState::plot) {
		if (GetCurrentPlot() && GetCurrentPlot()->GetMainEntity() == this) {
			if (_currentPlots.size() > 0 && !_currentPlots[0]->GetIsExclusive() && _currentPlots[0]->GetInvolvedInPlot().Num() > 0) {

				UOEntity* notorious = nullptr;
				notorious = _currentPlots[0]->GetInvolvedInPlot()[0];
				if (notorious->IsValidItem()) {

					for (UOEntity* entity : _currentPlots[0]->GetInvolvedInPlot()) {
						if (!entity || !entity->IsValidItem() || entity->GetMainPlotEntity() == this)
							continue;

						if (entity->GetNotoriety() > notorious->GetNotoriety())
							notorious = entity;
					}

					_currentPlots[0]->ChangeMainEntity(notorious);
					_currentPlots[0]->DeleteFromInvolved(notorious);
					notorious->AddCurrentPlotWithPriority(_currentPlots[0]);
					notorious->SetMainPlotEntity(nullptr);

					for (UOEntity* entity : _currentPlots[0]->GetInvolvedInPlot()) {
						if (!entity || !entity->IsValidItem())
							continue;
						entity->SetMainPlotEntity(notorious);
					}
					_currentPlots.erase(_currentPlots.begin());
				}
			}
		}
		else if (GetMainPlotEntity()->IsValidItem() && GetMainPlotEntity()->GetCurrentPlot()) {
			GetMainPlotEntity()->GetCurrentPlot()->DeleteFromInvolved(this);
		}
			
	}
	
	if (_currentPlots.size() > 0) {
		int size = _currentPlots.size();
		_plotGenerator->ChangeCurrentPlotsInAction(-size);
	}
	
	ClearState();
	GetEntityAIController()->behaviorTree->StopTree();
	GetEntityAIController()->UnPossess();
	//GetEntityAIController()->Destroy();
	_entityAIController = nullptr;

	// Deleted from notorious entities
	_plotGenerator->DeleteNotorious(this);
	//Deleted from all entities
	_plotGenerator->allEntities.Remove(this);
	
	//Clears possessions and removes this as owner
	while (_possessions.size() > 0){
		_possessions[0]->GetOwnable()->RemoveOwner(this);
		_possessions.erase(_possessions.begin());
	}

	// Everybody forgets their relation and nearbyness with this
	for (auto entity : _plotGenerator->allEntities) {
		if (entity->GetRelationWith(this))
			entity->DeleteRelation(this);

		TArray<UOEntity*>* entityNearbies = nullptr;
		entityNearbies = entity->GetNearbyEntitiesPtr();
		if (entityNearbies->Contains(this))
			entityNearbies->Remove(this);
		TArray<UOEntity*>* entitiesItReactedTo = nullptr;
		entitiesItReactedTo = entity->GetEntitiesReactingToPtr();
		if (entitiesItReactedTo->Contains(this))
			entitiesItReactedTo->Remove(this);
			
	}

	ThreadManager::SomeoneDied(this);

	_brain = NULL;
}



// Generates notifications for reactiveness
// Generates Plot Reports and changes in ontology
// Deletes entity from the relationships lists of its relationships
void UOEntity::IHaveBeenKilledBySomeone(UOEntity * killer)
{
	//   P L O T S
	//	Search in relationships for those who appreciate entity, change ontology if required and send reports

	vector<ORelation*> relations = GetRelationships();

	for (ORelation* o : relations) {

		UOEntity* otherEntity = o->GetOtherEntity();

		if (!otherEntity || !otherEntity->IsValidItem() || otherEntity == killer)
			continue;

		ORelation* relationFromOther = otherEntity->GetRelationWith(this);
		
		if (relationFromOther){
			if (relationFromOther->GetAppreciation() > ORelation::HIGH_APPRECIATION) {
				ORelation* othersRelationWithKiller = otherEntity->GetRelationWith(killer);

				if (!othersRelationWithKiller) {
					othersRelationWithKiller = otherEntity->AddRelationship(killer);
				}

				if (othersRelationWithKiller) {
					othersRelationWithKiller->ChangeAppreciation(-relationFromOther->GetAppreciation());

					if (othersRelationWithKiller->GetAppreciation() < ORelation::LOW_APPRECIATION) {
						SendReport(new Report(othersRelationWithKiller, TypeOfPlot::aggressive, this));
					}

					//Since something bad has happened, let's check the overall hate against the enemy race
					if (killer->GetRace() != ERace::R_Troll && killer->GetRace()!=_race && _plotGenerator->WarCount == 0) {
						if (_plotGenerator->GetOverallHateAgainstRace(_race == ERace::R_Human ? ERace::R_Goblin : ERace::R_Human) < ORelation::LOW_APPRECIATION) {
							for(UOEntity* e : _plotGenerator->GetNotoriousEntities())
								SendReport(new Report(e, TypeOfPlot::world));
						}
					}
				}
			}
		}
	}

	CastNotify((UItem*) this, killer, ENotify::N_Obliterated);

	// A T E N C I � N   A   E S T O
	/*for (ORelation* o : relations) {
		o->GetOtherEntity()->DeleteRelation(this);
	}*/
}

void UOEntity::IHaveBeenHelpedBySomeone(UOEntity * helper, UItem* motivation, int oldAppreciation, int newAppreciation)
{
	if (oldAppreciation-10 < ORelation::HIGH_APPRECIATION && newAppreciation > ORelation::HIGH_APPRECIATION) {
		Report* newReport = new Report(GetRelationWith(helper), TypeOfPlot::thankful, motivation);
	}
}

void UOEntity::SetMainPlotEntity(UOEntity* mpe) {
	if(mpe && mpe->IsValidItem())
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

void UOEntity::AddCurrentPlotWithPriority(BasePlot * bp)
{
	_currentPlots.insert(_currentPlots.begin(), bp);
}


bool UOEntity::CheckValidPersonality(TypeOfPlot type) {

	switch (type) {

	case TypeOfPlot::aggressive:
		return (_personality->GetAggressiveness() > 50 && _personality->GetBraveness() > 50);

	case TypeOfPlot::possessive:
		return (_personality->GetMaterialist() > 50 && _personality->GetPride() > 50);

	case TypeOfPlot::thankful:
		return (_personality->GetKindness() > 50 && _personality->GetSocial() > 50);

	case TypeOfPlot::preventive:
		return (_personality->GetAstute() > 50);

	case TypeOfPlot::world:
		return true;

	case TypeOfPlot::resources:
		return true;

	default:
		return true;
	}
}




// S T A T E S

void UOEntity::SetIdleGraph(Graph* g) {
	_idleGraph = g;
}

Graph * UOEntity::GetIdleGraph()
{
	return _idleGraph;
}

Graph* UOEntity::GetBrain()
{
	return &_brain;
}

void UOEntity::SetLastNode(Node* n) {
	_lastNode = n;
}
Node* UOEntity::GetLastNode() {
	return _lastNode;
}

UOEntity::AIState UOEntity::GetCurrentState() {
	return _currentState;
}

void UOEntity::SetNumb() {
	SetState(AIState::numb);
}

void UOEntity::SetState(AIState s) {

	_isNumb = false;

	if (_currentState != s || !_brain.Peek()) {  //HACK/MOCK/CHAPUZA
		_currentState = s;

		switch (_currentState) {
		case AIState::idle:
		{
			_brain = Graph(*_idleGraph);
			if (_brain.Peek()->nBlackboard.daytime >= 0) {
				while (_brain.Peek()->nBlackboard.daytime <= _plotGenerator->dayTime) {
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

		case AIState::plot:
		{
			if (_currentPlots.size() > 0 && GetCurrentPlot()->GetMainEntity() == this) {
				_brain = GetCurrentPlot()->GetGraph();
				_currentPlots[0]->SavePlotToFile(Utilities::SavePath, Utilities::PlotFile);
				_currentPlots[0]->PrintSentence(_plotGenerator, _currentPlots[0]->GetMotivationName(), _currentPlots[0]->GetAmbition());

				BasePlot* plotToReact = _currentPlots[0]->ConsiderReactions();
				if (plotToReact) {
					plotToReact->GetMainEntity()->AddCurrentPlotWithPriority(plotToReact);
					plotToReact->GetMainEntity()->RethinkState();
				}
			}
			else {
				if (_mainPlotEntity && _mainPlotEntity->GetCurrentPlot()) {

					_brain = _mainPlotEntity->GetCurrentPlot()->GetGraph();
					if (!_brain.IsLastNode()) {
						_brain.NextNode();

						Node* comeToEntity = new Node();
						comeToEntity->SetNodeType(NodeType::goToActor);
						comeToEntity->SetActor(_mainPlotEntity->GetOwner());
						_brain.AddSplitSecondNode(comeToEntity);
					}
				}
			}
		}
		break;

		case AIState::react:
		{
			Graph* g = _currentReacts[0];
			_brain = *g;
		}
		break;

		case AIState::numb:
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

		if(GetEntityAIController())
			GetEntityAIController()->SetState(_currentState);
	}

	if (_currentState != AIState::numb) 
		ExecuteGraph();
}


//   N O T I F Y

void UOEntity::ReceiveNotify(UItem* predicate, UOEntity* subject, ENotify notifyType, FString notifyID) {
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Yellow, TEXT("Subject: ") + subject->GetItemName());
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Yellow, TEXT("Predicate: ") + predicate->GetItemName());
	if (IsDead || IsPlayer || !predicate || !predicate->IsValidItem() || !subject || !subject->IsValidItem() || this == subject || predicate->IsA<UOEntity>() && this == ((UOEntity*)predicate))
		return;
	
	for (FString id : _knownNotifyIDs) {
		if (id == notifyID)
			return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow, GetItemName() + TEXT(" received a Notify! ") + notifyID);

	_knownNotifyIDs.push_back(notifyID);

	if (notifyType == ENotify::N_Released) {
		if(subject->IsPlayer && !predicate->IsA<UOEntity>()){
			if (
				//Distance B - A
				(GetOwner()->GetActorLocation() - subject->GetOwner()->GetActorLocation()).Size() < 500.f
				&&
				//Distance B - O
				(GetOwner()->GetActorLocation() - predicate->GetOwner()->GetActorLocation()).Size() < 500.f
				&&
				//InSight B - A
				FMath::Acos(FVector::DotProduct(GetOwner()->GetActorForwardVector().GetSafeNormal(), (subject->GetOwner()->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal())) < PI / 4.f
				&&
				//InSight B - O
				FMath::Acos(FVector::DotProduct(GetOwner()->GetActorForwardVector().GetSafeNormal(), (predicate->GetOwner()->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal())) < PI / 4.f
				&&
				//InSight A - B
				FMath::Acos(FVector::DotProduct(subject->GetOwner()->GetActorForwardVector().GetSafeNormal(), (GetOwner()->GetActorLocation() - subject->GetOwner()->GetActorLocation()).GetSafeNormal())) < PI / 4.f
				)
			{
				// It's A Present!!
				// Para miiii?
				// Para t���!!
				ReceivePresent((UOOwnable*) predicate, subject);
			}
		}	
	}
	else if(notifyType == ENotify::N_Damaged) {

		ORelation* relationWithSubject = GetRelationWith(subject);
		if (!relationWithSubject)
			relationWithSubject = AddRelationship(subject);

		if (relationWithSubject) {
			if (predicate->IsA<UOEntity>()) {
				ORelation* relationWithPredicate = GetRelationWith((UOEntity*)predicate);
				if (!relationWithPredicate)
					relationWithPredicate = AddRelationship((UOEntity*)predicate);
				if(relationWithPredicate)
					relationWithSubject->ChangeAppreciation((50 - relationWithPredicate->GetAppreciation()) / 3);
			}
			else if (predicate->IsA<UOEdification>()) {
				OOwnership* ownershipOfPredicate = GetOwnershipWith((UOOwnable*)predicate);
				if (ownershipOfPredicate) {
					relationWithSubject->ChangeAppreciation(-ownershipOfPredicate->GetWorth()*GetMaterialist() / 100 / 3);
				}
			}
		}

		Retaliation(1, predicate, subject);
	
		// SUBJECT HELPS KILLING / DESTROYING PREDICATE ?
		Node* currentNode = _brain.Peek();
		if (currentNode
			&& (currentNode->GetNodeType() == NodeType::attack || currentNode->GetNodeType() == NodeType::destroy)
			&& (predicate == currentNode->nBlackboard.entity || predicate == currentNode->nBlackboard.edification)
			|| _lastNode
			&& (_lastNode->GetNodeType() == NodeType::attack || _lastNode->GetNodeType() == NodeType::destroy)
			&& (predicate == _lastNode->nBlackboard.entity || predicate == _lastNode->nBlackboard.edification)
			) {
			//YOU HELPED ME KILL/DESTROY HIM/IT!
			ORelation* r = GetRelationWith(subject);
			if (!r)
				r = AddRelationship(subject);
			if (r) {
				int oldAppreciation = r->GetAppreciation();

				r->ChangeAppreciation(HELP_APPRECIATION_INCREASE);
				r->ChangeFear(HELP_FEAR_DECREASE);

				IHaveBeenHelpedBySomeone(subject, predicate, oldAppreciation, r->GetAppreciation());
			}
		}
	}
	else if (notifyType == ENotify::N_Obliterated) {

		ORelation* relationWithSubject = GetRelationWith(subject);
		if (!relationWithSubject)
			relationWithSubject = AddRelationship(subject);

		if (relationWithSubject) {
			if (predicate->IsA<UOEntity>()) {
				if (subject->IsPlayer) {
					ORelation* predicatesRelationWithSubject = ((UOEntity*)predicate)->GetRelationWith(subject);
					if (!predicatesRelationWithSubject)
						predicatesRelationWithSubject = ((UOEntity*)predicate)->AddRelationship(subject);
					if(predicatesRelationWithSubject)
						relationWithSubject->ChangeFear(predicatesRelationWithSubject->GetAppreciation());
				}
				else {
					ORelation* subjectsRelationWithPredicate = subject->GetRelationWith((UOEntity*)predicate);
					if (!subjectsRelationWithPredicate)
						subjectsRelationWithPredicate = subject->AddRelationship((UOEntity*)predicate);
					if(subjectsRelationWithPredicate)
						relationWithSubject->ChangeFear(subjectsRelationWithPredicate->GetAppreciation());
				}
			}
			else if (predicate->IsA<UOEdification>()) {
				relationWithSubject->ChangeFear((100 - relationWithSubject->GetAppreciation()) / 2);
			}

			Retaliation(2, predicate, subject);

			// SUBJECT HELPS KILLING / DESTROYING PREDICATE
			Node* currentNode = _brain.Peek();
			if (currentNode
				&& (currentNode->GetNodeType() == NodeType::attack || currentNode->GetNodeType() == NodeType::destroy)
				&& (predicate == currentNode->nBlackboard.entity || predicate == currentNode->nBlackboard.edification)
				|| _lastNode
				&& (_lastNode->GetNodeType() == NodeType::attack || _lastNode->GetNodeType() == NodeType::destroy)
				&& (predicate == _lastNode->nBlackboard.entity || predicate == _lastNode->nBlackboard.edification)
				) {
				//YOU HELPED ME KILL/DESTROY HIM/IT!
				ORelation* r = GetRelationWith(subject);
				if (!r)
					r = AddRelationship(subject);
				if (r) {
					int oldAppreciation = r->GetAppreciation();

					r->ChangeAppreciation(HELP_APPRECIATION_INCREASE);
					r->ChangeFear(HELP_FEAR_DECREASE);

					IHaveBeenHelpedBySomeone(subject, predicate, oldAppreciation, r->GetAppreciation());
				}
			}
		}
	}
	else if (notifyType == ENotify::N_Grabbed)
	{
		ORelation* relationWithSubject = GetRelationWith(subject);
		if (!relationWithSubject)
			relationWithSubject = AddRelationship(subject);
		if (relationWithSubject && predicate->IsA<UOOwnable>()) {
			OOwnership* ownership = GetOwnershipWith((UOOwnable*)predicate);
			if (ownership) {
				if (ownership->GetWorth() + GetMaterialist() > relationWithSubject->GetAppreciation() + GetKindness()) {
					//Decrease appreciation
					relationWithSubject->ChangeAppreciation(-ownership->GetWorth() / 2);
				}
			}
		}
		else if (predicate->IsA<UOEntity>()) { // the troll grabbing someone
			ORelation* relationWithPredicate = GetRelationWith((UOEntity*)predicate);
			if (!relationWithPredicate)
				relationWithPredicate = AddRelationship((UOEntity*)predicate);
			if (relationWithPredicate && relationWithPredicate->GetAppreciation() > relationWithSubject->GetAppreciation()) {
				//Decrease appreciation
				relationWithSubject->ChangeAppreciation(-relationWithPredicate->GetAppreciation()/3);
				//Increase fear
				relationWithSubject->ChangeFear(10);
			}
		}
		Retaliation(0, predicate, subject);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, GetItemName() + TEXT(" doesn't understand what's happening"));
	}
	RethinkState();
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

/// relation modified before calling this function
/// grade E [1,3]
/// 0: no violence   1: violence   2: serious violence
/// possible outcomes:
///		attack, flee, get weapon, stop fight, (ask for help)
void UOEntity::Retaliation(int grade, UItem* predicate, UOEntity* subject) {

	if (!predicate || !predicate->IsValidItem() || !subject || !subject->IsValidItem())
		return;

	if (this == predicate || this == subject || IsPlayer)
		return;

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Yellow, GetItemName() + TEXT(" is going to retaliate against ") + subject->GetItemName() + TEXT(" doing something of grade ") + FString::SanitizeFloat(grade) + TEXT(" over ") + predicate->GetItemName());

	ORelation* relationWithSubject = GetRelationWith(subject);
	if (!relationWithSubject)
		relationWithSubject = AddRelationship(subject);
	
	ORelation* relationWithPredicate = nullptr;
	if (relationWithSubject) {	
		if (predicate->IsA<UOEntity>()) {
			relationWithPredicate = GetRelationWith((UOEntity*)predicate);
			if (!relationWithPredicate)
				relationWithPredicate = AddRelationship((UOEntity*)predicate);
		}
	}
	
	OOwnership* ownershipOfPredicate = nullptr;
	ORelation* relationWithFriendOwner = nullptr;
	if (predicate->IsA<UOOwnable>()) {
		ownershipOfPredicate = GetOwnershipWith((UOOwnable*)predicate);
		if (!ownershipOfPredicate) {
			for (UOEntity* owner : ((UOOwnable*)predicate)->GetOwners()) {
				if (!owner || !owner->IsValidItem())
					continue;

				if (!relationWithFriendOwner) {
					relationWithFriendOwner = GetRelationWith(owner);
				}
				else {
					ORelation* newPredicate = GetRelationWith(owner);
					if (newPredicate) {
						if (newPredicate->GetAppreciation() > relationWithFriendOwner->GetAppreciation())
							relationWithFriendOwner = newPredicate;
					}
				}
			}
		}
	}

	Graph* graph = new Graph();
	Node* n;
	bool mayAttackSubject = true;

	if (relationWithPredicate) { // someone bothering someone
		if (relationWithPredicate->GetAppreciation() > 75-15*grade || relationWithPredicate->GetRespect() > 75-10*grade) {
			if (relationWithSubject->GetAppreciation() > 50 + 10 * grade || relationWithSubject->GetRespect() > 50 + 15 * grade) {
				n = new Node();
				n->SetNodeType(NodeType::stopFight); n->SetEntity(subject); n->SetItem(predicate); n->SetHighPriority();
				graph->AddNode(n);
				if (((UOEntity*)predicate)->GetBrain()->Peek()->GetNodeType() == NodeType::attack && ((UOEntity*)predicate)->GetBrain()->Peek()->nBlackboard.entity == subject) {
					n = new Node();
					n->SetNodeType(NodeType::stopFight); n->SetEntity((UOEntity*)predicate); n->SetItem(subject); n->SetHighPriority();
					graph->AddNode(n);
				}
				mayAttackSubject = false;
			}
		}
	}
	else if(ownershipOfPredicate) { // someone messing with something you own
		if (grade == 0) {
			if (relationWithSubject->GetAppreciation() + GetKindness() < ownershipOfPredicate->GetWorth() + GetMaterialist()) {
				n = new Node();
				n->SetNodeType(NodeType::steal); n->SetEntity(subject); n->SetOwnable((UOOwnable*)predicate); n->SetHighPriority();
				graph->AddNode(n);
				if(GetAggressiveness() < 50)
					mayAttackSubject = false;
			}
		}
		else if (grade == 1) {
			if (relationWithSubject->GetAppreciation() > ownershipOfPredicate->GetWorth() || relationWithSubject->GetRespect() > ownershipOfPredicate->GetWorth()+10) {
				n = new Node();
				n->SetNodeType(NodeType::stopFight); n->SetEntity(subject); n->SetItem(predicate); n->SetHighPriority();
				graph->AddNode(n);
				mayAttackSubject = false;
			}
		}
		//else if grade == 2 always attack
	}
	else if (relationWithFriendOwner) { // someone messing with something a buddy of yours owns
		if (relationWithFriendOwner->GetAppreciation() > 75 - 10 * grade || relationWithFriendOwner->GetRespect() > 75 - 5 * grade) {
			if (grade == 0) {
				// Nothing to do here
			}
			else if (grade == 1) {
				if (relationWithSubject->GetAppreciation() > relationWithFriendOwner->GetAppreciation() || relationWithSubject->GetRespect() > relationWithFriendOwner->GetRespect()) {
					n = new Node();
					n->SetNodeType(NodeType::stopFight); n->SetEntity(subject); n->SetItem(predicate); n->SetHighPriority();
					graph->AddNode(n);
					mayAttackSubject = false;
				}
			}
			else if (grade == 2) {
				if (relationWithSubject->GetAppreciation() > relationWithFriendOwner->GetAppreciation() || relationWithSubject->GetRespect() > relationWithFriendOwner->GetRespect()) {
					mayAttackSubject = false;
				}
			}
		}
		else {
			mayAttackSubject = false;
		}
	}



	if (mayAttackSubject) {
		if (GetBraveness() > relationWithSubject->GetFear() || GetPride() > relationWithSubject->GetFear() + relationWithSubject->GetRespect()) {
			if (GetAggressiveness() > 90 - 40*grade) {
				n = new Node();
				n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetHighPriority();
				graph->AddNode(n);
			}
			else if (HasGrabbedItem()) {
				n = new Node();
				n->SetNodeType(NodeType::releaseItem); n->SetHighPriority();
				graph->AddNode(n);
			}
			n = new Node();
			n->SetNodeType(NodeType::attack); n->SetEntity(subject); n->SetHighPriority();
			graph->AddNode(n);
		}
		else {//if (GetBraveness() < relationWithSubject->GetFear()) {
			n = new Node();
			n->SetNodeType(NodeType::flee); n->SetActor(subject->GetOwner()); n->SetHighPriority();
			graph->AddNode(n);
		}
	}

	if (graph->GetGraphSize() > 0) {
		AddReactGraph(graph, subject);
		RethinkState();
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
	_isEntityAttacking = false;
	_isEntityBuilding = false;
	_isEntityCultivating = false;
	_isEntityMining = false;
	if(GetEntityAIController())
		GetEntityAIController()->SetNode(_brain.Peek());
	//_entityAIController->ExecuteNode();
}



// If a node can't be completed or is the last one, plot is considered completed
void UOEntity::NodeCompleted(bool completedOk) {
	if (completedOk && !_brain.IsLastNode()) {
		_brain.NextNode();
		if (_currentState == AIState::plot && GetCurrentPlot()
			&& GetCurrentPlot()->GetMainEntity() == this){
			Graph* plotGraph = _currentPlots[0]->GetGraphPointer();
			if (!IsA<UOCivilian>()
				|| plotGraph->Peek()->GetNodeType() != NodeType::grab
				&& plotGraph->Peek()->GetNodeType() != NodeType::get)
			{
				plotGraph->NextNode();
			}
		}	
		else if (_currentState == AIState::react) {
			if (!IsA<UOCivilian>()
				|| _currentReacts[0]->Peek()->GetNodeType() != NodeType::grab
				&& _currentReacts[0]->Peek()->GetNodeType() != NodeType::get) 
			{
				_currentReacts[0]->NextNode();
			}
		}
	}
	else {
		ClearState();
	}

	_isEntityAttacking = false;
	_isEntityCultivating = false;
	_isEntityMining = false;
	_isEntityBuilding = false;

	RethinkState();
}

void UOEntity::ClearState()
{
 	if (_currentState == AIState::plot) {
		if (GetCurrentPlot() && GetCurrentPlot()->GetMainEntity() == this) {

			for (UOEntity* e : _currentPlots[0]->GetInvolvedInPlot()) {
				if (!e || !e->IsValidItem() && e->GetMainPlotEntity() == this)
					continue;
				e->SetMainPlotEntity(nullptr);
				e->RethinkState();	
			}
			_currentPlots.erase(_currentPlots.begin());
			_plotGenerator->ChangeCurrentPlotsInAction(-1);
		}
		else if(GetMainPlotEntity()->IsValidItem()) {
			if(GetMainPlotEntity()->GetCurrentPlot()) 
				GetMainPlotEntity()->GetCurrentPlot()->DeleteFromInvolved(this);
			SetMainPlotEntity(nullptr);
		}
	}
	else if (_currentState == AIState::react) {
		_currentReacts.erase(_currentReacts.begin());
		if(_entitiesReactingTo.Num() > 0)
			_entitiesReactingTo.RemoveAt(0);
	}

	_currentState = AIState::restart;
	_isEntityAttacking = false;
	_isEntityBuilding = false;
	_isEntityCultivating = false;
	_isEntityMining = false;
}


void UOEntity::AddInstantNode(Node * n)
{
	_brain.AddInstantNode(n);
}

void UOEntity::AddInstantReactGraph(Graph * g)
{
	vector<Graph*> aux;
	aux.push_back(g);
	for (Graph* graph : _currentReacts)
		aux.push_back(graph);
	_currentReacts = aux;
}

void UOEntity::AddReactGraph(Graph * g, UOEntity* e)
{
	if (!_entitiesReactingTo.Contains(e)) {
		_currentReacts.push_back(g);
		_entitiesReactingTo.Add(e);
	}
}
void UOEntity::AddInconditionalReactGraph(Graph * g)
{
	_currentReacts.push_back(g);
	_entitiesReactingTo.Add(this);
}

vector<Graph*> UOEntity::GetReacts()
{
	return _currentReacts;
}

void UOEntity::RethinkState() {	
	if (GetCurrentState() != AIState::numb && !IsDead) {

		// Current action is of high priority
		if (_currentState != AIState::restart && _brain.GetGraphSize() >0 && _brain.Peek() && _brain.Peek()->nBlackboard.isHighPriority) {
			SetState(_currentState);
		}

		// React pending of execution
		else if (!_currentReacts.empty()) {
			//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Yellow, GetOwner()->GetActorLabel() + TEXT(" wants to react!"));
			SetState(AIState::react);
		}

		// Plot pending of execution
		else if (!_currentPlots.empty() || _mainPlotEntity)
			SetState(AIState::plot);

		// Nothing important to do, continue to idle
		else
			SetState(AIState::idle);
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
	if (!item || !item->IsValidItem() || item->IsA<UOEdification>())
		return;

	if (GetStrength() / 2.0f >= item->GetMass()) { // should this block be managed on UOEntity::GrabItem()?
		
		// Empty hands
		if (HasGrabbedItem())
			ReleaseGrabbedItem();

		// if this is concretely an Ownable
		if (item->IsA<UOOwnable>()) {
			UOOwnable* ownable = (UOOwnable*)item;
			// Tell others to stop trying to <grab> this
			for (UOEntity* grabber : ownable->GetGrabbers()) {
				if (!grabber || !grabber->IsValidItem())
					continue;

				if (grabber->GetBrain()->Peek()->GetNodeType() == NodeType::grab
					&& (GetOwner()->GetActorLocation() - grabber->GetOwner()->GetActorLocation()).Size() <= 2000) {
					grabber->NodeCompleted(false);
				}
			}

			// Add ownership?
			bool canOwn = true;
			for (UOEntity* owner : ((UOOwnable*)item)->GetOwners()) {
				if (!owner || !owner->IsValidItem())
					continue;

				ORelation* r = GetRelationWith(owner);
				if (r && r->GetAppreciation() > 50){
					canOwn = false;
					break;
				}
			}
			if (canOwn) {
				AddPossession(ownable);
			}
		}

		// Actually GRAB the thing
		_grabbedItem = item;
		AActor* grabbedItemActor = _grabbedItem->GetOwner();
		grabbedItemActor->SetActorEnableCollision(false);
		AttachToSocket(grabbedItemActor, "RightHandSocket");
		grabbedItemActor->UpdateOverlaps(true);
		grabbedItemActor->OnActorBeginOverlap.Add(HitFunc);
		if(_grabbedItem->IsA<UOOwnable>())
			((UOOwnable*)_grabbedItem)->SetIsGrabbed(true);
	}
}

void UOEntity::ReleaseGrabbedItem() {
	if (_grabbedItem->IsValidItem()) {
		AActor* grabbedItemActor = nullptr;
		grabbedItemActor = _grabbedItem->GetOwner();
		//dejar
		if (grabbedItemActor) {
			if (_grabbedItem->IsValidItem() && _grabbedItem->IsA<UOOwnable>())
				((UOOwnable*)_grabbedItem)->SetIsGrabbed(false);
			CastNotify(_grabbedItem, this, ENotify::N_Released);
			if (_grabbedItem->IsValidItem() && !_grabbedItem->IsA<UOEntity>()) {
				grabbedItemActor->SetActorLocation(FVector(grabbedItemActor->GetActorLocation().X, grabbedItemActor->GetActorLocation().Y, 0));
			}
			grabbedItemActor->OnActorBeginOverlap.Remove(HitFunc);
			grabbedItemActor->DetachRootComponentFromParent(true);
			grabbedItemActor->SetActorEnableCollision(true);
		}
		_grabbedItem = nullptr;
	}
}


void UOEntity::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("I have been hit!!: ") + GetItemName());

}

void UOEntity::AttachToSocket(AActor* target, string socket) {

	if (((ACharacter*)GetOwner())->GetMesh()->DoesSocketExist(socket.c_str()))
		target->AttachRootComponentTo(((ACharacter*)GetOwner())->GetMesh(), socket.c_str(), EAttachLocation::SnapToTarget, true);
	else GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("Trying to Attach to non-existing socket"));
}



void UOEntity::FinishedFindingNearbyEntities(/*TArray<UOEntity*> entitiesFound*/) {
	_searchingNearbyEntities = false;
	//_nearbyEntities = entitiesFound;
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::FromHex("#88FF11"), GetItemName() + TEXT(" found ") + FString::SanitizeFloat(_nearbyEntities.Num()) + TEXT(" nearby entities."));
	int startNearbyEntitiesCount = _nearbyEntities.Num();
	int i = 0;
	bool somethingNewToDo = false;
	while (i < _nearbyEntities.Num()) {
		if (startNearbyEntitiesCount != _nearbyEntities.Num()) {
			entitiesFinderDelay = ENTITIES_FINDER_DELAY;
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("NearbyEntities num changed"));
			return;
		}
		UOEntity* e = _nearbyEntities[i];

		if (!e || !e->IsValidItem())
			continue;

		ORelation* r = GetRelationWith(e);
		if (!r) {
			r = AddRelationship(e);
		}

		if (r->GetAppreciation() < 50) {
			r->ChangeAppreciation(1);
		}
		if (r->GetFear() > 50) {
			r->ChangeFear(-1);
		}

		if (!_entitiesReactingTo.Contains(e)) {
			Graph* react = new Graph();
			Node* n;

			if (r->GetAppreciation() > 75) {
				if (e->IsPlayer)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hi ") + e->GetItemName() + TEXT(", I greet you!"));
			}
			else if (r->GetRespect() > 75) {
				if (e->IsPlayer)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hello ") + e->GetItemName() + TEXT(", I salute you!"));
			}
			else if (r->GetFear() > 50 && r->GetFear() > GetBraveness()) {
				n = new Node();
				n->SetNodeType(NodeType::flee); n->SetActor(e->GetOwner()); n->SetHighPriority();
				react->AddNode(n);
			}
			else if (r->GetAppreciation() < 25 && GetAggressiveness() > 33 || GetAggressiveness() > 95 && r->GetAppreciation() < 90) {
				n = new Node();
				n->SetNodeType(NodeType::get); n->SetAffordableUse(OntologicFunctions::AffordableUse::weapon); n->SetHighPriority();
				react->AddNode(n);
				n = new Node();
				n->SetNodeType(NodeType::attack); n->SetEntity(e); n->SetHighPriority();
				react->AddNode(n);
			}
			else if (r->GetAppreciation() < 50 && r->GetRespect() < 50) {
				// Look at stranger
				if (e->IsPlayer)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("I do not trust you, strager..."));
			}

			if (react->GetGraphSize() > 0) {
				AddReactGraph(react, e);
				somethingNewToDo = true;
			}
		}
		i++;
	}
	if (somethingNewToDo)
		RethinkState();
}


UOEntity* UOEntity::FindPrey() {
	for (UOEntity* e : _nearbyEntities) {
		if (!e || !e->IsValidItem())
			continue;

		//if (!IsCivilian || !e->IsCivilian) {}		// in case we want Civilians to be able to hunt
		ORelation* r = GetRelationWith(e);
		if (!r)
			r = AddRelationship(e);
		if (r->GetAppreciation() < 50					// Is not your Friend
			&& (e->GetStrength() <= _strength / 2.f	// bastly inferior
			|| e->GetIntegrity() <= 50				// weakened
			|| e->_nearbyEntities.Num() <= 2))		// isolated
			return e;
	}
	return NULL;
}

void UOEntity::ReceivePresent(UOOwnable* present, UOEntity* giver) {
	if (!present || !present->IsValidItem() || !giver || !giver->IsValidItem())
		return;

	// DO STUFF
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, GetItemName() + TEXT(" received a ") + present->GetItemName() + TEXT(" as a present from ") + giver->GetItemName());
	ORelation* r = GetRelationWith(giver);
	if (!r) //THIS SHOULDN'T HAPPEN
		r = AddRelationship(giver);
	int oldAppreciation = r->GetAppreciation();
	//int presentValue = present->GetIntrinsicValue();
	int presentValue = 0;
	presentValue += 10 * (int)((UOOwnable*)present)->GetRarity();
	OntologicFunctions* ontf = new OntologicFunctions();
	for (int i = OntologicFunctions::AffordableUse::weapon; i < OntologicFunctions::AffordableUse::build; i++) {
		presentValue += ontf->GetAffordance((OntologicFunctions::AffordableUse)i, present, this) * 5.f / 100.f;
	}
	r->ChangeAppreciation(presentValue);
	r->ChangeFear(-presentValue / 2.f);


	// HELPS SOMEONE GETTING SOMETHING
	Node* currentNode = _brain.Peek();

	if (currentNode
		&& (currentNode->GetNodeType() == NodeType::get || currentNode->GetNodeType() == NodeType::grab || currentNode->GetNodeType() == NodeType::steal)
		&& present == currentNode->nBlackboard.ownable
		|| _lastNode
		&& (_lastNode->GetNodeType() == NodeType::get || _lastNode->GetNodeType() == NodeType::grab || _lastNode->GetNodeType() == NodeType::steal)
		&& present == _lastNode->nBlackboard.ownable
		) {
		//YOU HELPED ME GET THE ITEM!
		r->ChangeAppreciation(HELP_APPRECIATION_INCREASE);
		r->ChangeFear(HELP_FEAR_DECREASE);
	}

	IHaveBeenHelpedBySomeone(giver, present, oldAppreciation, r->GetAppreciation());


	Graph* graph = new Graph();
	Node* n = new Node();
	n->SetNodeType(NodeType::grab); n->SetOwnable(present);
	graph->AddNode(n);
	if (GetHome()->IsValidItem()) {
		n = new Node();
		n->SetNodeType(NodeType::goToActor); n->SetActor(GetHome()->GetOwner());
		graph->AddNode(n);
		n = new Node();
		n->SetNodeType(NodeType::releaseItem);
		graph->AddNode(n);
	}

	AddInconditionalReactGraph(graph);
}