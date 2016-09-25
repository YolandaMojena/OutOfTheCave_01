// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include <algorithm>
#include "Ontology/OEntity.h"
#include "Ontology/ORelation.h"
#include "Ontology/OOwnable.h"
#include "Ontology/Item.h"
#include "Ontology/OOwnership.h"
#include "NarrativeGeneration/Utilities.h"
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

	Report(ORelation* newRelation, TypeOfPlot type, UItem* motivation);
	Report(OOwnership* newOwnership, TypeOfPlot type, UItem* motivation);
	Report(UOEntity* entity, TypeOfPlot type);

	~Report();

	void SaveReportToFile(const FString SaveDirectory, const FString FileName);

	UOEntity* GetReportEntity();
	UOEntity* GetTargetEntity();
	UOOwnable* GetTargetOwnable();
	ORelation* GetNewRelation();
	OOwnership* GetNewOwnership();
	//UItem* GetMotivation();
	FString GetMotivationRace();
	FString GetMotivationName();
	ReportTag GetTag();
	TypeOfPlot GetType();

	//void RemoveTagFromReport(BasePlot::TypeOfPlot type);

	struct ReportNotoriety
	{
		bool operator()(Report& A, Report& B) const
		{
			// Inverted compared to std::priority_queue - higher priorities float to the top
			return A.GetReportEntity()->GetNotoriety() >= B.GetReportEntity()->GetNotoriety();
		}
	};


private:
	UOEntity* _reportEntity = nullptr;
	UOEntity* _targetEntity = nullptr;
	UOOwnable* _targetOwnable = nullptr;
	//UItem _motivation;

	ReportTag _tag;
	TypeOfPlot _type;

	FString _motivationRace;
	FString _motivationName;
};
