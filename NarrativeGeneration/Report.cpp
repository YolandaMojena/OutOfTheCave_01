// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Report.h"


Report::Report(ORelation * newRelation, BasePlot::TypeOfPlot type, UItem* motivation)
{
	_reportEntity =  newRelation->GetEntity();
	_targetEntity = newRelation->GetOtherEntity();
	_newRelation = newRelation;
	_type = type;
	_motivation = motivation;

	_tag = ReportTag::relation;
}

Report::Report(OOwnership* newOwnership, BasePlot::TypeOfPlot type, UItem* motivation)
{
	_reportEntity = newOwnership->GetOwner();
	_targetOwnable = newOwnership->GetOwnable();
	_newOwnership = newOwnership;
	_type = type;
	_motivation = motivation;

	_tag = ReportTag::ownership;
}

//Temporal constructor for world events
Report::Report(UOEntity * reportEntity)
{
	_reportEntity = reportEntity;
	_tag = ReportTag::world;
}

Report::~Report()
{
}

void Report::PrintReport() {

	if (GetTag() == Report::ReportTag::relation) {

		FString report = "Report from entity " + GetReportEntity()->GetOwner()->GetActorLabel() + " about entity " + GetTargetEntity()->GetOwner()->GetActorLabel() + " caused by " + GetMotivation()->GetOwner()->GetActorLabel();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, report);
	}
	else if (GetTag() == Report::ReportTag::ownership) {

		FString report = "Report from entity " + GetReportEntity()->GetOwner()->GetActorLabel() + " about ownable " + GetTargetOwnable()->GetOwner()->GetActorLabel() + " caused by: " + GetMotivation()->GetOwner()->GetActorLabel();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, report);
	}
}

/*void Report::RemoveTagFromReport(BasePlot::TypeOfPlot type) {

	for (BasePlot::TypeOfPlot t : _types) {
		if (t == type) {
			_types.erase(remove(_types.begin(), _types.end(), t), _types.end());
			break;
		}
	}
}*/


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
BasePlot::TypeOfPlot Report::GetType() {
	return _type;
}
UItem* Report::GetMotivation() {
	return _motivation;
}
