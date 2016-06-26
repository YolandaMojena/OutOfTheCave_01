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
	enum class State : uint8 {
		idle UMETA(DisplayName = "idle"),
		plot UMETA(DisplayName = "plot"),
		react UMETA(DisplayName = "react"),
		numb UMETA(DisplayName = "numb")
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

	bool GetIsNumb();
	void SetIsNumb(bool value);

	void ReceiveDamage(float damage, UOEntity* damager);
	bool CheckValidPersonality(TypeOfPlot type);
	void SendReport(Report* newReport);

	State GetCurrentState();
	void SetState(State s, Graph* g = nullptr);
	void SetIdleGraph(Graph* g);
	UOEntity* GetMainPlotEntity();
	void SetMainPlotEntity(UOEntity* mpe);
	ERace GetRace();
	FString GetRaceString();
	void SetRace(ERace race);
	EJob GetJob();
	void SetJob(EJob);

	void SetAIController(AEntityAIController* eaic);
	void ExecuteGraph();
	void NodeCompleted(bool completedOk);
	void AddInstantNode(Node* n);

	vector<UOOwnable*> GetInventory();
	void StoreInInventory(UOOwnable* o);
	bool RemoveFromInventory(UOOwnable* o);
	bool RemoveFromInventory(int i);

	void Attack();
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
	void SetStrength(float st);
	void SetSpeed(float sd);
	void SetAgility(float ag);
	void GenerateTraits();

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

	vector<ORelation*> _relationships;
	vector<UOEntity*> _potentialRelationships;
	vector<OOwnership*> _possessions;
	vector<OOwnership*> _materialDesires;
	vector<OTerritory*> _landlord;

	UOEntity* _attacker;
	float MIN_INTEGRITY = 20.0f;
	float _attackCooldown;
	USkeletalMeshComponent* _skelMesh;
	
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