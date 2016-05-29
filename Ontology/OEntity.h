// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
#include "Ontology/OOwnable.h"
//#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OTerritory.h"
#include "Ontology/OPersonality.h"
#include <vector>
#include "OEntity.generated.h"

using namespace std;

class ORelation;
class APlotGenerator;

/**
 * 
 */
UCLASS()
class OUTOFTHECAVE_01_API UOEntity : public UItem
{
	GENERATED_BODY()

public:
	UOEntity();
	UOEntity(OPersonality* personality);

	void BeginPlay() override;

	vector<ORelation*> GetRelationships();
	vector<OOwnership*> GetPossessions();
	vector<OTerritory*> GetTerritories();
	OPersonality* GetPersonality();

	void AddRelationship(ORelation* newRelation);
	void AddPossession(OOwnership* newOwnership);
	void AddTerritory(OTerritory* newTerritory); 

	ORelation* GetRelationWith(UOEntity* other);
	OOwnership* GetOwnershipWith(UOOwnable* other);

	APlotGenerator* plotGenerator;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	OPersonality* _personality;
	
private:

	// Must be called when changes are detected in the state of the ontology to add plots
	void ChangeOfStateInOntology(ORelation* newRelation);
	void ChangeOfStateInOntology(OOwnership* newOwnership);
	void ChangeOfStateInOntology(OTerritory* newTerritory);

	// Allows to load a texture from a given path
	static UTexture2D* LoadTextureFromPath(const FString& Path);

	vector<ORelation*> _relationships;
	vector<OOwnership*> _possessions;
	vector<OTerritory*> _landlord;

	string _currentIconPath;
};
