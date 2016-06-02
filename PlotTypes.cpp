// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "StringCollection.h"
#include "PlotTypes.h"


//ATTACK PLOT
//**************************************************************************************

AttackPlot::AttackPlot(UOEntity* plotEntity) : BasePlot(plotEntity) {

	_name = "AttackPlot";
	plotTypes = { TypeOfPlot::aggressive };

	vector<Node*> graphNodes = { new BeginNode("Begin",strings.WAIT_ICON), new GoToNode("GoTo", strings.GOTO_ICON), new AttackNode("Attack", strings.ATTACK_ICON), new GoToNode("Return", strings.GOTO_ICON) };
	vector<Node::Arc> graphArcs = { Node::Arc("Begin", "GoTo", ""), Node::Arc("GoTo", "Attack", "") };

	BuildGraph(graphNodes, graphArcs);
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

	vector<Node*> graphNodes = { new BeginNode("Begin", strings.WAIT_ICON), new GoToNode("GoTo", strings.GOTO_ICON), new GatherNode("Gather", strings.GATHER_ICON), new GoToNode("Return", strings.GOTO_ICON) };
	vector<Node::Arc> graphArcs = { Node::Arc("Begin", "GoTo", ""), Node::Arc("GoTo", "Gather", "") };

	BuildGraph(graphNodes, graphArcs);
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




