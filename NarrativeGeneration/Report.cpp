// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Report.h"


Report::Report(ORelation * newRelation, TypeOfPlot type, UItem* motivation)
{
	_reportEntity =  newRelation->GetEntity();
	_targetEntity = newRelation->GetOtherEntity();
	_newRelation = newRelation;
	_type = type;
	//_motivation = motivation;

	_motivationName = motivation->GetItemName();
	if (motivation->IsA<UOEntity>())
		_motivationRace = ((UOEntity*)motivation)->GetRaceString();
	else
		_motivationRace = "";

	_tag = ReportTag::relation;
}

Report::Report(OOwnership* newOwnership, TypeOfPlot type, UItem* motivation)
{
	_reportEntity = newOwnership->GetOwner();
	_targetOwnable = newOwnership->GetOwnable();
	_newOwnership = newOwnership;
	_type = type;
	//_motivation = motivation;

	_motivationName = motivation->GetItemName();
	if (motivation->IsA<UOEntity>())
		_motivationRace = ((UOEntity*)motivation)->GetRaceString();
	else
		_motivationRace = "";

	_tag = ReportTag::ownership;
}

//Constructor for world events
Report::Report(UOEntity* entity, TypeOfPlot type)
{
	_reportEntity = entity;
	_type = type;
	_tag = ReportTag::world;
}

Report::~Report()
{
}

void Report::SaveReportToFile(const FString SaveDirectory, const FString FileName)
{
	FString report;

	switch (_type) {
	case TypeOfPlot::aggressive:
		report += "Report type: aggressive\n";
		break;

	case TypeOfPlot::possessive:
		report += "Report type: possessive\n";
		break;

	case TypeOfPlot::resources:
		report += "Report type: resources\n";
		break;

	case TypeOfPlot::thankful:
		report += "Report type: thankful\n";
		break;

	case TypeOfPlot::preventive:
		report += "Report type: preventive\n";
		break;

	case TypeOfPlot::world:
		report += "Report type: world\n";
		break;
	}

	if (GetTag() == Report::ReportTag::relation) {

		 /*"Report from entity " + GetReportEntity()->GetOwner()->GetActorLabel() + " about entity " + GetTargetEntity()->GetOwner()->GetActorLabel() + " caused by " + GetMotivation()->GetOwner()->GetActorLabel();*/
		report += "Change in relation report\n";
		report += "Report entity: " + GetReportEntity()->GetItemName() + "\n";
		report += "Target entity: " + GetTargetEntity()->GetItemName() + "\n";
		report += "Motivation: " + GetMotivationName() + "\n";
		//report += "Report type: " + FindObject<UEnum>(ANY_PACKAGE, TEXT("TypeOfPlot"), true)->GetEnumName((int32)_type);
	}
	else if (GetTag() == Report::ReportTag::ownership) {

		/*FString report = "Report from entity " + GetReportEntity()->GetOwner()->GetActorLabel() + " about ownable " + GetTargetOwnable()->GetOwner()->GetActorLabel() + " caused by: " + GetMotivation()->GetOwner()->GetActorLabel();*/
		report += "Change in ownership report\n";
		report += "Report entity: " + GetReportEntity()->GetItemName() + "\n";
		report += "Target ownable: " + GetTargetOwnable()->GetItemName() + "\n";
		report += "Motivation: " + GetMotivationName() + "\n";
		//report += "Report type: " + FindObject<UEnum>(ANY_PACKAGE, TEXT("TypeOfPlot"), true)->GetEnumName((int32)_type);
	}
	else report += "Massive change in world state report\n";

	report += "\n\n";
	Utilities::SaveStringToFile(report, SaveDirectory, FileName);
}


UOEntity* Report::GetReportEntity() {
	return _reportEntity;
}
UOEntity* Report::GetTargetEntity() {
	return _targetEntity;
}
UOOwnable* Report::GetTargetOwnable(){
	return _targetOwnable;
}
ORelation* Report::GetNewRelation() {
	return _newRelation;
}
OOwnership* Report::GetNewOwnership() {
	return _newOwnership;
}
Report::ReportTag Report::GetTag() {
	return _tag;
}
TypeOfPlot Report::GetType() {
	return _type;
}
FString Report::GetMotivationName() {
	return _motivationName;
}
FString Report::GetMotivationRace() {
	return _motivationRace;
}
