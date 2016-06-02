// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "Ontology/OEntity.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OOwnership.h"
#include "BasePlot.h"


/**
 * 
 */
class OUTOFTHECAVE_01_API Report
{
public:

	enum ReportTag {
		relation, ownership, world
	};

	Report(ORelation* newRelation, BasePlot::TypeOfPlot type, UItem* motivation);
	Report(OOwnership* newOwnership, BasePlot::TypeOfPlot type, UItem* motivation);
	Report(UOEntity* reportEntity);

	~Report();

	UOEntity* GetReportEntity();
	UOEntity* GetTargetEntity();
	UOOwnable* GetTargetOwnable();
	ORelation* GetNewRelation();
	OOwnership* GetNewOwnership();
	UItem* GetMotivation();
	ReportTag GetTag();
	BasePlot::TypeOfPlot GetType();


private:
	UOEntity* _reportEntity;
	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
	ORelation* _newRelation;
	OOwnership* _newOwnership;
	UItem* _motivation;

	ReportTag _tag;
	BasePlot::TypeOfPlot _type;

	// -1, 0 or 1
	int _priority;
};
