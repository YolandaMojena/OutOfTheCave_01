// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Report.h"


Report::Report(UOEntity * reportEntity, UOEntity * target, ORelation * newRelation)
{
	_reportEntity = reportEntity;
	_targetEntity = target;
	_newRelation = newRelation;
	_tag = ReportTag::relation;
}

Report::Report(UOEntity * reportEntity, UOOwnable * target, OOwnership* newOwnership)
{
	_reportEntity = reportEntity;
	_targetOwnable = target;
	_newOwnership = newOwnership;
	_tag = ReportTag::ownership;
}

//Temporal constructor for world events
Report::Report(UOEntity * reportEntity, UOEntity * target)
{
	_reportEntity = reportEntity;
	_targetEntity = target;
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
