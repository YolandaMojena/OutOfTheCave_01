// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "StringCollection.h"
#include "Ontology/OEntity.h"
#include "PlotTypes.h"


//ATTACK PLOT
//**************************************************************************************

AttackPlot::AttackPlot(UOEntity* plotEntity) : BasePlot(plotEntity) {

	_name = "AttackPlot";
	plotTypes = { TypeOfPlot::aggressive };

	BuildGraph();
}

AttackPlot::~AttackPlot() {}

string AttackPlot::BuildSentence() {

	return TCHAR_TO_UTF8(*(plotEntity->GetOwner()->GetActorLabel() + "is attacking"));
}

void AttackPlot::GatherTargets() {

	_sentence = BuildSentence();
}

void AttackPlot::ConsiderReactions() {

}


//GATHER PLOT
//**************************************************************************************

GatherPlot::GatherPlot(UOEntity* plotEntity) : BasePlot(plotEntity) {

	_name = "GatherPlot";
	plotTypes = { TypeOfPlot::resources };


	BuildGraph();
}

GatherPlot::~GatherPlot() {}

string GatherPlot::BuildSentence() {

	FString entityName = plotEntity->GetOwner()->GetActorLabel();
	string conversion1(TCHAR_TO_UTF8(*entityName));


	return "Entity " + conversion1 + " is gathering ";

}

void GatherPlot::GatherTargets() {


	_sentence = BuildSentence();
}

void GatherPlot::ConsiderReactions() {

}




