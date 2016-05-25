// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/Item.h"
//#include "Ontology/ORelation.h"
#include "Ontology/OOwnership.h"
#include "Ontology/OTerritory.h"
#include "Ontology/OPersonality.h"
#include <vector>
#include "OEntity.generated.h"

using namespace std;

class ORelation;

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

	vector<ORelation*> GetRelationships();
	vector<OOwnership*> GetPossessions();
	vector<OTerritory*> GetTerritories();
	OPersonality* GetPersonality();

	void AddRelationship(ORelation* newRelation);
	void AddPossession(OOwnership* newOwnership);
	void AddTerritory(OTerritory* newTerritory);


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entity)
	OPersonality* _personality;

	
private:
	vector<ORelation*> _relationships;
	vector<OOwnership*> _possessions;
	vector<OTerritory*> _landlord;
	
};
