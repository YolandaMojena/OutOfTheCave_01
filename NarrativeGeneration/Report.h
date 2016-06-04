// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "Ontology/OEntity.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OOwnership.h"
#include "BasePlot.h"

using namespace std;

/**
 * 
 */
class OUTOFTHECAVE_01_API Report
{
public:

	enum ReportTag {
		relation, ownership, world
	};

	Report(ORelation* newRelation, vector<BasePlot::TypeOfPlot> types, UItem* motivation);
	Report(OOwnership* newOwnership, vector<BasePlot::TypeOfPlot> types, UItem* motivation);
	Report(UOEntity* reportEntity);

	~Report();

	void PrintReport(Report* newReport);

	UOEntity* GetReportEntity();
	UOEntity* GetTargetEntity();
	UOOwnable* GetTargetOwnable();
	ORelation* GetNewRelation();
	OOwnership* GetNewOwnership();
	UItem* GetMotivation();
	ReportTag GetTag();
	vector<BasePlot::TypeOfPlot> GetTypes();


private:
	UOEntity* _reportEntity;
	UOEntity* _targetEntity;
	UOOwnable* _targetOwnable;
	ORelation* _newRelation;
	OOwnership* _newOwnership;
	UItem* _motivation;

	ReportTag _tag;
	vector<BasePlot::TypeOfPlot> _types;

	// -1, 0 or 1
	int _priority;
};
