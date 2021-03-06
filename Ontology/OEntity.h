// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
#include "Ontology/OTerritory.h"
#include "Ontology/OPersonality.h"
#include <vector>
#include "BasePlot.h"
#include <algorithm>
#include <string>

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "OEntity.generated.h"

using namespace std;

class ORelation;
class OOwnership;
class UOOwnable;
class APlotGenerator;
class Report;
class AEntityAIController;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOEntity : public UItem
{
	GENERATED_BODY()

	enum State{
		idle, plot, react
	};
	
	

public:
	UOEntity();
	UOEntity(OPersonality* personality);

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	vector<ORelation*> GetRelationships();
	vector<OOwnership*> GetPossessions();
	vector<OTerritory*> GetTerritories();
	OPersonality* GetPersonality();
	int GetNotoriety();

	void AddRelationship(ORelation* newRelation);
	void AddRelationship(UOEntity* newEntity);
	void AddPossession(OOwnership* newOwnership);
	void AddPossession(UOOwnable* newOwnable);
	void AddTerritory(OTerritory* newTerritory);
	void AddDesire(OOwnership* newOwnership);
	void AddDesire(UOOwnable* newOwnable);


	bool IsInSight(AActor* actor);
	void OwnableNotify(UOOwnable* ownable, UOEntity* entity, UItem::_NotifyTag tag, bool grito, string notifyID);
	void EntityNotify(UOEntity* pasiva, UOEntity* activa, UItem::_NotifyTag tag, bool grito, string notifyID);
	
	ORelation* GetRelationWith(UOEntity* other);
	OOwnership* GetOwnershipWith(UOOwnable* other);
	void DeleteRelation(UOEntity* relation);
	void DeletePossession(UOOwnable* possession);
	void DeleteDesire(UOOwnable* desire);

	bool GetIsDead();

	void ReceiveDamage(float damage, UOEntity* damager);
	bool CheckValidPersonality(BasePlot::TypeOfPlot type);
	void SendReport(Report* newReport);

	// It must be considered whether if the entity is the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	bool IsPlayer;

	// All entities will send reports to the plotGenerator situated in the game world
	APlotGenerator* plotGenerator;

	vector<BasePlot*> currentPlot;
	Graph* brain;

	UPROPERTY(EditAnywhere, Category = Behaviour)
	class UBehaviorTree* entityBehaviorTree;

	void SetAIController(AEntityAIController* eaic);
	void ExecuteGraph();
	void NodeCompleted(bool completedOk);
	
private:
	
	State _currentState;
	
	void Die();
	void IHaveBeenKilledBySomeone(UOEntity* killer);

	vector<ORelation*> _relationships;
	vector<OOwnership*> _possessions;
	vector<OOwnership*> _materialDesires;
	vector<OTerritory*> _landlord;

	UOEntity* _attacker;

	bool _isDead = false;

	float MIN_INTEGRITY = 20.0f;

	bool _highPriority = false;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	OPersonality* _personality;

	int _notoriety = 0;
	int _notifyID;

	Graph* _idleGraph;

	void SetState(State s, Graph* g = nullptr);

	float _currentTime = 10;

	AEntityAIController* _entityAIController;
};
