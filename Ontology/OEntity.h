// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
#include "Ontology/OOwnable.h"
//#include "Ontology/ORelation.h"
#include "Ontology/OTerritory.h"
#include "Ontology/OPersonality.h"
#include <vector>
#include <algorithm>
#include "OEntity.generated.h"

using namespace std;

class ORelation;
class OOwnership;
class UOOwnable;
class APlotGenerator;
class Report;

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
	
	State* currentState;

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

	

	ORelation* GetRelationWith(UOEntity* other);
	OOwnership* GetOwnershipWith(UOOwnable* other);
	void DeleteRelation(UOEntity* relation);
	void DeletePossession(UOOwnable* possession);

	bool GetIsDead();

	void ReceiveDamage(float damage, UOEntity* damager);

		// Must be called when changes are detected in the state of the ontology to add plots
	void ChangeOfStateInOntology(ORelation* newRelation);
	void ChangeOfStateInOntology(OOwnership* newOwnership);

	// Leave territories out for now
	//void ChangeOfStateInOntology(OTerritory* newTerritory);

	void SendReport(Report* newReport);

	// It must be considered whether if the entity is the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	bool IsPlayer;

	// All entities will send reports to the plotGenerator situated in the game world
	APlotGenerator* plotGenerator;

	

	
private:

	void Die();
	void IHaveBeenKilledBySomeone(UOEntity* killer);

	vector<ORelation*> _relationships;
	vector<OOwnership*> _possessions;
	vector<OTerritory*> _landlord;

	UOEntity* _attacker;

	bool _isDead = false;

	float MIN_INTEGRITY = 20.0f;

	bool _highPriority = false;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	OPersonality* _personality;

	int _notoriety = 0;
};
