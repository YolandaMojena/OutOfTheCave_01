// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Report.h"


Report::Report(ORelation * newRelation, vector<BasePlot::TypeOfPlot> types, UItem* motivation)
{
	_reportEntity =  newRelation->GetEntity();
	_targetEntity = newRelation->GetOtherEntity();
	_newRelation = newRelation;
	_types = types;
	_motivation = motivation;

	_tag = ReportTag::relation;
}

Report::Report(OOwnership* newOwnership, vector<BasePlot::TypeOfPlot> types, UItem* motivation)
{
	_reportEntity = newOwnership->GetOwner();
	_targetOwnable = newOwnership->GetItem();
	_newOwnership = newOwnership;
	_types = types;
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

void Report::PrintReport(Report* newReport) {

	if (newReport->GetTag() == Report::ReportTag::relation) {

		FString report = "Report from entity " + newReport->GetReportEntity()->GetOwner()->GetActorLabel() + " about entity " + newReport->GetTargetEntity()->GetOwner()->GetActorLabel() + ". Reports a change caused by " + newReport->GetMotivation()->GetOwner()->GetActorLabel();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, report);
	}
	else if (newReport->GetTag() == Report::ReportTag::ownership) {

		FString report = "Report from entity " + newReport->GetReportEntity()->GetOwner()->GetActorLabel() + " about ownable " + newReport->GetTargetOwnable()->GetOwner()->GetActorLabel() + ". Reports a change caused by: " + newReport->GetMotivation()->GetOwner()->GetActorLabel();
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, report);
	}
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
vector<BasePlot::TypeOfPlot> Report::GetTypes() {
	return _types;
}
UItem* Report::GetMotivation() {
	return _motivation;
}
