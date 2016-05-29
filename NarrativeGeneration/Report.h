// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "Ontology/OEntity.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OOwnership.h"


/**
 * 
 */
class OUTOFTHECAVE_01_API Report
{
public:

	enum ReportTag {
		relation, ownership, world
	};

	Report(UOEntity* reportEntity, UOEntity* target, ORelation* newRelation);
	Report(UOEntity* reportEntity, UOOwnable* target, OOwnership* newOwnership);
	Report(UOEntity* reportEntity, UOEntity* target);

	~Report();

	UOEntity* GetReportEntity();
	UOEntity* GetTargetEntity();
	UOOwnable* GetTargetOwnable();
	ORelation* GetNewRelation();
	OOwnership* GetNewOwnership();
	ReportTag GetTag();


private:
	UOEntity* _reportEntity;
	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
	ORelation* _newRelation;
	OOwnership* _newOwnership;

	ReportTag _tag;

	// -1, 0 or 1
	int _priority;
};
