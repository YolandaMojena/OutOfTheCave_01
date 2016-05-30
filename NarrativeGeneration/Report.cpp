// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Report.h"


Report::Report(ORelation * newRelation, BasePlot::TypeOfPlot type)
{
	_reportEntity =  newRelation->GetEntity();
	_targetEntity = newRelation->GetOtherEntity();
	_newRelation = newRelation;
	_tag = ReportTag::relation;
	_type = type;
}

Report::Report(OOwnership* newOwnership, BasePlot::TypeOfPlot type)
{
	_reportEntity = newOwnership->GetOwner();
	_targetOwnable = newOwnership->GetItem();
	_newOwnership = newOwnership;
	_tag = ReportTag::ownership;
	_type = type;
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
