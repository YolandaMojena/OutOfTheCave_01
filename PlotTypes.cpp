// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "StringCollection.h"
#include "Ontology/OEntity.h"
#include "Ontology/OOwnable.h"
#include "PlotTypes.h"


//ATTACK PLOT
//**************************************************************************************

AttackPlot::AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity) : BasePlot(plotEntity) {

	_name = "AttackPlot";
	_targetEntity = targetEntity;
	plotTypes = { TypeOfPlot::aggressive };
	_sentence = BuildSentence();
	_discrete = false;

	BuildGraph();

	if (!_discrete) GatherTargets();
}

AttackPlot::~AttackPlot() {}

string AttackPlot::BuildSentence() {

	return TCHAR_TO_UTF8(*("Entity: " + plotEntity->GetOwner()->GetActorLabel() + " is attacking " + _targetEntity->GetOwner()->GetActorLabel()));
}

void AttackPlot::GatherTargets() {	
}

void AttackPlot::ConsiderReactions() {
}

void AttackPlot::BuildGraph() {

	_plotGraph = new Graph();

	Node* attackNode = new Node();
	attackNode->name = strings.ATTACK_NODE;
	attackNode->PopulateBlackboard(plotEntity);
	attackNode->SetIconPath(strings.ATTACK_ICON);
	_plotGraph->AddNode(attackNode);
}


//GATHER PLOT
//**************************************************************************************

GatherPlot::GatherPlot(UOEntity* plotEntity, UOOwnable* targetResource) : BasePlot(plotEntity) {

	_name = "GatherPlot";
	_targetResource = targetResource;
	plotTypes = { TypeOfPlot::resources };
	_sentence = BuildSentence();
	_discrete = false;


	if (!_discrete) GatherTargets();
	//BuildGraph();
}

GatherPlot::~GatherPlot() {}

string GatherPlot::BuildSentence() {

	return TCHAR_TO_UTF8(*("Entity " + plotEntity->GetOwner()->GetActorLabel() + " is gathering " + _targetResource->GetOwner()->GetActorLabel()));
}

void GatherPlot::GatherTargets() {
}

void GatherPlot::ConsiderReactions() {
}

void GatherPlot::BuildGraph() {

}




