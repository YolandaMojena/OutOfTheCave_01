// Fill out your copyright notice in the Description page of Project Settings.
#define LOCTEXT_NAMESPACE "FText Namespace" 
#pragma once

#include "Ontology/Item.h"
#include "Ontology/OTerritory.h"
#include "Ontology/OPersonality.h"
#include "OwnableSpawner.h"
#include <vector>
#include "BasePlot.h"
#include <algorithm>
#include <string>
#include <cstdlib>

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
	enum class AIState : uint8 {
		idle UMETA(DisplayName = "idle"),
		plot UMETA(DisplayName = "plot"),
		react UMETA(DisplayName = "react"),
		numb UMETA(DisplayName = "numb"),
		restart UMETA(DisplayName = "restart")
	};

	struct EntityNotoriety
	{
		bool operator()(UOEntity& A, UOEntity& B) const
		{
			// Inverted compared to std::priority_queue - higher priorities float to the top
			return A.GetNotoriety() >= B.GetNotoriety();
		}
	};

	//Graph* GetBrain();
	//void SetBrain(Graph* b);

	static float MIN_INTEGRITY;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FString _hands_name = FString(TEXT("hands"));
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_centerOfMass = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		float _hands_edgeLength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		float _hands_edgeSharpness = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_funcDir = FVector(0, 0, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_funcPos = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_grabDir = FVector(0, 0, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_grabPos = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		float _hands_mass = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		float _hands_maxLength = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		float _hands_spikes = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		float _hands_spiky = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		float _hands_toughness = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		float _hands_volume = 450;


	vector<BasePlot*> GetCurrentPlots();
	BasePlot* GetCurrentPlot();
	void AddCurrentPlot(BasePlot* bp);
	void AddCurrentPlotWithPriority(BasePlot* bp);

	UOEntity();
	UOEntity(OPersonality* personality);

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityCultivating;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityMining;


	// It must be considered whether if the entity is the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
		bool IsPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isNumb;

	UPROPERTY(EditAnywhere, Category = Behaviour)
	class UBehaviorTree* entityBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityTraits)
		float _strength;	// mainly attack damage and max item weight to grab | human soldier ~30
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityTraits)
		float _speed;		// mainly movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EntityTraits)
		float _agility;		// mainly attack cooldown
	

	vector<ORelation*> GetRelationships();
	vector<OOwnership*> GetPossessions();
	vector<OTerritory*> GetTerritories();
	OPersonality* GetPersonality();
	int GetNotoriety();
	void ChangeNotoriety(int value);
	UFUNCTION(BlueprintCallable, Category = "Entity")
		float GetStrength();
	UFUNCTION(BlueprintCallable, Category = "Entity")
		float GetSpeed();
	UFUNCTION(BlueprintCallable, Category = "Entity")
		float GetAgility();

	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetKindness();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetAggressiveness();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetBraveness();

	UFUNCTION(BlueprintCallable, Category = "EntityRelations")
		float GetAppreciationTo(UOEntity* ent);
	UFUNCTION(BlueprintCallable, Category = "EntityRelations")
		float GetFearTo(UOEntity* ent);
	UFUNCTION(BlueprintCallable, Category = "EntityRelations")
		float GetRespectTo(UOEntity* ent);

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		UBehaviorTree* BehaviorTree;
	AEntityAIController* GetEntityAIController();


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
	
	ORelation* GetRelationWith(UOEntity* other);
	OOwnership* GetOwnershipWith(UOOwnable* other);
	void DeleteRelation(UOEntity* relation);
	void DeletePotentitalRelationship(UOEntity* entity);
	void DeletePossession(UOOwnable* possession);
	void DeleteDesire(UOOwnable* desire);

	bool GetIsNumb();
	void SetIsNumb(bool value);

	void ReceiveDamage(float damage, UOEntity* damager);
	bool CheckValidPersonality(TypeOfPlot type);
	void SendReport(Report* newReport);

	AIState GetCurrentState();
	void SetIdleGraph(Graph* g);
	Graph* GetIdleGraph();
	Graph* GetBrain();
	UOEntity* GetMainPlotEntity();
	void SetMainPlotEntity(UOEntity* mpe);
	void RethinkState();
	
	ERace GetRace();
	FString GetRaceString();
	void SetRace(ERace race);
	EJob GetJob();
	void SetJob(EJob);
	AActor* GetCurrentTarget();
	void SetCurrentTarget(AActor* item);
	float GetAppreciationToOtherRace();
	UOEntity* GetMostHated();
	

	void SetAIController(AEntityAIController* eaic);
	void SetPlotGenerator();
	void ExecuteGraph();
	void NodeCompleted(bool completedOk);
	void ClearState(bool completedOk);
	//void AddInstantNode(Node* n);
	void AddInstantHelpNode(Node* n);
	void AddInstantReact(Graph* g);
	vector<Graph*> GetReacts();

	void ReceiveNotify(UItem* predicate, UOEntity* subject, ENotify notifyType, FString notifyID);

	vector<UOOwnable*> GetInventory();
	void StoreInInventory(UOOwnable* o);
	void GrabFromInventory(UOOwnable* o);
	bool RemoveFromInventory(UOOwnable* o);
	bool RemoveFromInventory(int i);
	void SpawnFromInventory(UOOwnable* o);
	void SpawnFromInventory(int i);
	void ReleaseInventory();

	void Attack();
	bool StealFromInventory(UOOwnable* o, UOEntity* buggler);
	void DamageEdification(UOEdification* targetEdification);
	UFUNCTION(BlueprintCallable, Category = "Entity")
		void EndAttack();
	void RebuildEdification(UOEdification* home);
	void StopRebuildEdification();
	bool IsEntityAttacking();
	float GetAttackCooldown();
	//void SetIsEntityAttacking(bool attacking);

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
	void SetState(AIState s); //, Graph* g = nullptr
	void SetStrength(float st);
	void SetSpeed(float sd);
	void SetAgility(float ag);
	void GenerateTraits();

	void Die();
	void IHaveBeenKilledBySomeone(UOEntity* killer);
	void IHaveBeenHelpedBySomeone(UOEntity* helper, UItem* motivation);

	AIState _currentState = AIState::idle;
	ERace _race;
	EJob _job;

	vector<BasePlot*> _currentPlots;
	UOEntity* _mainPlotEntity = nullptr;
	Graph _brain;
	Graph* _idleGraph;
	vector<Graph*> _currentReacts;
	AEntityAIController* _entityAIController;
	AActor* _currentTarget;
	UOEntity* _mostHatedEntity;

	vector<ORelation*> _relationships;
	vector<UOEntity*> _potentialRelationships;
	vector<OOwnership*> _possessions;
	vector<OOwnership*> _materialDesires;
	vector<OTerritory*> _landlord;

	UOEntity* _attacker;
	float _attackCooldown;
	USkeletalMeshComponent* _skelMesh;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	OPersonality* _personality;

	int _notoriety = 0;
	vector<FString> _knownNotifyIDs;

	vector<UOOwnable*> _inventory;
	UItem* _grabbedItem;

private:

	class UOOwnable* _deadOwnable;
	float _notifyDeadline = 0.f;
	void CleanKnownNotifyIDs(float deltaTime);
		
	UItem* _hands;

	AOwnableSpawner* _ownableSpawner;
};

#undef LOCTEXT_NAMESPACE 