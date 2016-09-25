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
class UOEdification;
class UOOwnable;
class APlotGenerator;
class Report;
class AEntityAIController;
class FNearbyEntitiesFinder;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OUTOFTHECAVE_01_API UOEntity : public UItem
{
	GENERATED_BODY()

public:

	UOEntity();
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
	static const int HELP_APPRECIATION_INCREASE = 10;
	static const int HELP_FEAR_DECREASE = -5;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FString _hands_name = "hands";
	// FE5
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_mass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_volume;
	// FE3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_toughness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_edgeSharpness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_edgeLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_spiky;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_nbrSpikes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_elongation;
	// RESTRICTED _r
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		int32 _hands_r_maxLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_r_grabPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_r_grabDir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_r_funcPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_r_funcDir; //Think of an axe!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_r_funcPlane; //Think of an axe!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hands)
		FVector _hands_r_centerOfMass;

	UOOwnable* GetHands();

	vector<BasePlot*> GetCurrentPlots();
	BasePlot* GetCurrentPlot();
	void AddCurrentPlot(BasePlot* bp);
	void AddCurrentPlotWithPriority(BasePlot* bp);
	//UOEntity(OPersonality* personality);

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
		FString TraitsString;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityCultivating;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isEntityMining;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Entity)
		bool _isNumb;

	// It must be considered whether if the entity is the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
		bool IsPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
		bool IsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
		ERace _race;

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
	
	UFUNCTION(BlueprintCallable, Category = "Entity")
		TArray<UOEntity*> GetNearbyEntities();
		
	TArray<UOEntity*>* GetNearbyEntitiesPtr();
	TArray<UOEntity*>* GetEntitiesReactingToPtr();

	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetKindness();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetAggressiveness();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetBraveness();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetSocial();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetMaterialist();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetCurious();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetAstute();
	UFUNCTION(BlueprintCallable, Category = "EntityPersonality")
		float GetPride();

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
	ORelation* AddRelationship(UOEntity* newEntity);
	void AddPossession(OOwnership* newOwnership);
	void AddPossession(UOOwnable* newOwnable);
	void AddTerritory(OTerritory* newTerritory);
	bool DoesOwn(UOOwnable* ownable);
	bool DoesOwn(UItem* item);
	
	ORelation* GetRelationWith(UOEntity* other);
	OOwnership* GetOwnershipWith(UOOwnable* other);
	void DeleteRelation(UOEntity* relation);
	void DeletePossession(UOOwnable* possession);

	bool GetIsNumb();
	void SetNumb();

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
	void SetLastNode(Node* node);
	Node* GetLastNode();
	
	ERace GetRace();
	FString GetRaceString();
	void SetRace(ERace race);
	EJob GetJob();
	void SetJob(EJob);
	FVector GetGoingLocation();
	float GetAppreciationToOtherRace(ERace race);
	UOEntity* GetMostHated();
	
	void SetAIController(AEntityAIController* eaic);
	void SetPlotGenerator();
	void ExecuteGraph();
	void NodeCompleted(bool completedOk);
	void ClearState();
	void AddInstantNode(Node* n);
	void AddInstantReactGraph(Graph* g);
	void AddReactGraph(Graph* g, UOEntity* e);
	void AddInconditionalReactGraph(Graph* g);
	vector<Graph*> GetReacts();

	void ReceiveNotify(UItem* predicate, UOEntity* subject, ENotify notifyType, FString notifyID);
	void Retaliation(int grade, UItem* predicate, UOEntity* subject);

	void Attack();
	//void DamageEdification(UOEdification* targetEdification, FVector collisionPos);
	UFUNCTION(BlueprintCallable, Category = "Entity")
		void EndAttack();
	void RebuildEdification(UOEdification* home);
	void StopRebuildEdification();
	bool IsEntityAttacking();
	float GetAttackCooldown();
	//void SetIsEntityAttacking(bool attacking);
	
	void GenerateTraits();

	void GrabItem(UItem* item);
	UItem* GetGrabbedItem();
	bool HasGrabbedItem();
	void ReleaseGrabbedItem();
	TScriptDelegate<FWeakObjectPtr> HitFunc;
	void AttachToSocket(AActor* target, string socket);
	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ReceivePresent(UOOwnable* present, UOEntity* giver);

	UOEntity* FindPrey();
	void Die();
	
	UOEdification* GetHome();
	void SetHome(UOEdification* home);

	float _currentTime = 10;

	void FinishedFindingNearbyEntities(/*TArray<UOEntity*> entitiesFound*/);

	bool IsInitialized = false;
	
protected:
	void SetState(AIState s); //, Graph* g = nullptr
	void SetStrength(float st);
	void SetSpeed(float sd);
	void SetAgility(float ag);
	void ChangeStrength(float add);
	void ChangeSpeed(float add);
	void ChangeAgility(float add);
	
	void IHaveBeenKilledBySomeone(UOEntity* killer);
	void IHaveBeenHelpedBySomeone(UOEntity* helper, UItem* motivation, int oldAppreciation, int newAppreciation);

	AIState _currentState = AIState::idle;
	EJob _job;

	vector<BasePlot*> _currentPlots;
	UOEntity* _mainPlotEntity = nullptr;
	Graph _brain;
	Node* _lastNode = nullptr;
	Graph* _idleGraph = nullptr;
	vector<Graph*> _currentReacts;
	AEntityAIController* _entityAIController = nullptr;
	UOEntity* _mostHatedEntity = nullptr;

	vector<ORelation*> _relationships;
	vector<OOwnership*> _possessions;
	vector<OTerritory*> _landlord;

	UOEntity* _attacker = nullptr;
	float _attackCooldown;
	USkeletalMeshComponent* _skelMesh = nullptr;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	OPersonality* _personality = nullptr;

	int _notoriety = 0;
	vector<FString> _knownNotifyIDs;

	UItem* _grabbedItem = nullptr;
	TArray<UOEntity*> _nearbyEntities = TArray<UOEntity*>();
	TArray<UOEntity*> _entitiesReactingTo = TArray<UOEntity*>();

	bool _isRecognized = false;

private:

	const float ENTITIES_FINDER_DELAY = 5.f;
	float entitiesFinderDelay = 0.f;
	bool _searchingNearbyEntities = false;
	float _notifyDeadline = 0.f;
	void CleanKnownNotifyIDs(float deltaTime);
		
	UOEdification* _entityHome = nullptr;
};

#undef LOCTEXT_NAMESPACE 