// Fill out your copyright notice in the Description page of Project Settings.
#define LOCTEXT_NAMESPACE "FText Namespace" 
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
class OEdification;
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

public:

	UENUM(BlueprintType)
	enum class State : uint8 {
		idle UMETA(DisplayName = "idle"),
		plot UMETA(DisplayName = "plot"),
		react UMETA(DisplayName = "react")
	};

	//Graph* GetBrain();
	//void SetBrain(Graph* b);

	vector<BasePlot*> GetCurrentPlots();
	BasePlot* GetCurrentPlot();
	void AddCurrentPlot(BasePlot* bp);

	UOEntity();
	UOEntity(OPersonality* personality);

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityBuilding;

	// It must be considered whether if the entity is the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
		bool IsPlayer;

	UPROPERTY(EditAnywhere, Category = Behaviour)
	class UBehaviorTree* entityBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityTraits)
		float _strength;	// mainly attack damage and max item weight to grab | human soldier ~30
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityTraits)
		float _speed;		// mainly movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityTraits)
		float _agility;		// mainly attack cooldown
	void SetStrength(float st);
	void SetSpeed(float sd);
	void SetAgility(float ag);
	void GenerateTraits();

	vector<ORelation*> GetRelationships();
	vector<OOwnership*> GetPossessions();
	vector<OTerritory*> GetTerritories();
	OPersonality* GetPersonality();
	int GetNotoriety();
	void ChangeNotoriety(int value);
	int GetStrength();

	void AddRelationship(ORelation* newRelation);
	void AddRelationship(UOEntity* newEntity);
	void AddPotentialRelationship(UOEntity* newEntity);
	void AddPossession(OOwnership* newOwnership);
	void AddPossession(UOOwnable* newOwnable);
	void AddTerritory(OTerritory* newTerritory);
	void AddDesire(OOwnership* newOwnership);
	void AddDesire(UOOwnable* newOwnable);
	bool DoesOwn(UOOwnable* ownable);
	bool DoesOwn(UItem* item);

	bool IsInSight(AActor* actor);
	void OwnableNotify(UOOwnable* ownable, UOEntity* entity, UItem::_NotifyTag tag, bool grito, string notifyID);
	void EntityNotify(UOEntity* pasiva, UOEntity* activa, UItem::_NotifyTag tag, bool grito, string notifyID);
	
	ORelation* GetRelationWith(UOEntity* other);
	OOwnership* GetOwnershipWith(UOOwnable* other);
	void DeleteRelation(UOEntity* relation);
	void DeletePotentitalRelationship(UOEntity* entity);
	void DeletePossession(UOOwnable* possession);
	void DeleteDesire(UOOwnable* desire);

	bool GetIsDead();

	void ReceiveDamage(float damage, UOEntity* damager);
	bool CheckValidPersonality(TypeOfPlot type);
	void SendReport(Report* newReport);

	// All entities will send reports to the plotGenerator situated in the game world
	APlotGenerator* plotGenerator;

	State GetCurrentState();
	void SetState(State s, Graph* g = nullptr);
	void SetIdleGraph(Graph* g);
	UOEntity* GetMainPlotEntity();
	void SetMainPlotEntity(UOEntity* mpe);
	APlotGenerator* GetPlotGenerator();
	ERace GetRace();
	FString GetRaceString();
	void SetRace(ERace race);

	void SetAIController(AEntityAIController* eaic);
	void ExecuteGraph();
	void NodeCompleted(bool completedOk);
	void AddInstantNode(Node* n);

	vector<UOOwnable*> GetInventory();
	void StoreInInventory(UOOwnable* o);
	bool RemoveFromInventory(UOOwnable* o);
	bool RemoveFromInventory(int i);

	void Attack();
	void RebuildEdification(UOEdification* home);
	void StopRebuildEdification();
	bool GetIsEntityAttacking();
	void SetIsEntityAttacking(bool attacking);

	void GrabItem(UItem* item);
	UItem* GetGrabbedItem();
	bool HasGrabbedItem();
	void ReleaseGrabbedItem();
	TScriptDelegate<FWeakObjectPtr> HitFunc;
	void AttachToSocket(AActor* target, string socket);
	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	

	float _currentTime = 10;
	
protected:
	void Die();
	void IHaveBeenKilledBySomeone(UOEntity* killer);

	State _currentState;
	ERace _race;
	EJob _job;

	vector<BasePlot*> _currentPlots;
	UOEntity* _mainPlotEntity;
	Graph _brain;
	Graph* _idleGraph;
	AEntityAIController* _entityAIController;


	// All entities will send reports to the plotGenerator situated in the game world
	APlotGenerator* _plotGenerator;

	vector<ORelation*> _relationships;
	vector<UOEntity*> _potentialRelationships;
	vector<OOwnership*> _possessions;
	vector<OOwnership*> _materialDesires;
	vector<OTerritory*> _landlord;

	UOEntity* _attacker;
	bool _isDead = false;
	float MIN_INTEGRITY = 20.0f;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	OPersonality* _personality;

	int _notoriety = 0;
	int _notifyID;

	vector<UOOwnable*> _inventory;
	UItem* _grabbedItem;

private:

	class UOOwnable* _deadOwnable;

		
};

#undef LOCTEXT_NAMESPACE 