// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "StringCollection.h"
#include "Ontology/OEntity.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OEdification.h"
#include "PlotTypes.h"


//ATTACK PLOT
//**************************************************************************************

AttackPlot::AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_sentence = BuildSentence();
	_isExclusive = false;

	BuildGraph();
}

AttackPlot::~AttackPlot() {}

string AttackPlot::BuildSentence() {
	return TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is attacking " + _targetEntity->GetOwner()->GetActorLabel()));
}

void AttackPlot::BuildGraph() {

	_plotGraph = new Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph->AddNode(askForHelpNode);

	//GET WEAPON
	/*Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	n->SetArquetypeObject("espada")
	_plotGraph->AddNode(getNode);*/

	//GO TO VICTIM
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetEntity->GetOwner()->GetActorLocation());
	_plotGraph->AddNode(goToNode);

	//ATTACK
	Node* attackNode = new Node();
	attackNode->SetNodeType(NodeType::attack);
	attackNode->SetEntityB(_targetEntity);
	_plotGraph->AddNode(attackNode);
}


void AttackPlot::ConsiderReactions() {
}


//GATHER PLOT
//**************************************************************************************

GatherPlot::GatherPlot(UOEntity* plotEntity, UOOwnable* targetResource) : BasePlot(plotEntity) {

	_targetResource = targetResource;
	_sentence = BuildSentence();
	_isExclusive = false;

	BuildGraph();
}

GatherPlot::~GatherPlot() {}

string GatherPlot::BuildSentence() {
	return TCHAR_TO_UTF8(*("Entity " + _plotEntity->GetOwner()->GetActorLabel() + " is gathering " + _targetResource->GetOwner()->GetActorLabel()));
}

void GatherPlot::BuildGraph() {

}

void GatherPlot::ConsiderReactions() {
}


//DESTROY PLOT
//**************************************************************************************

DestroyPlot::DestroyPlot(UOEntity* plotEntity, UOOwnable* target) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_sentence = BuildSentence();
	_isExclusive = false;

	BuildGraph();
}

DestroyPlot::~DestroyPlot() {}

string DestroyPlot::BuildSentence() {
	return TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is destroying " + _targetOwnable->GetOwner()->GetActorLabel()));
}

void DestroyPlot::BuildGraph() {

	_plotGraph = new Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph->AddNode(askForHelpNode);

	//GET TOOLS
	/*Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	_plotGraph->AddNode(getNode);*/

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetOwnable->GetOwner()->GetActorLocation());
	_plotGraph->AddNode(goToNode);

	//DESTROY
	Node* destroyNode = new Node();
	destroyNode->SetNodeType(NodeType::destroy);
	destroyNode->SetOwnable(_targetOwnable);
	_plotGraph->AddNode(destroyNode);
}


void DestroyPlot::ConsiderReactions() {
}


//STEAL PLOT
//**************************************************************************************

StealPlot::StealPlot(UOEntity* plotEntity, UOEntity* who, UOOwnable* target) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_targetEntity = who;
	_sentence = BuildSentence();
	_isExclusive = true;

	BuildGraph();
}

StealPlot::~StealPlot() {}

string StealPlot::BuildSentence() {
	return TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is stealing " + _targetOwnable->GetOwner()->GetActorLabel() + " " + _targetEntity->GetOwner()->GetActorLabel()));
}

void StealPlot::BuildGraph() {

	_plotGraph = new Graph();

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetEntity->GetOwner()->GetActorLocation());
	_plotGraph->AddNode(goToNode);

	//STEAL
	Node* stealNode = new Node();
	stealNode->SetNodeType(NodeType::steal);
	stealNode->SetEntityB(_targetEntity);
	stealNode->SetOwnable(_targetOwnable);
	_plotGraph->AddNode(stealNode);
}

void StealPlot::ConsiderReactions() {
}


//BUILD PLOT
//**************************************************************************************

BuildPlot::BuildPlot(UOEntity* plotEntity, UOEdification* target) : BasePlot(plotEntity) {

	_targetEdification = target;
	_sentence = BuildSentence();
	_isExclusive = false;

	BuildGraph();
}

BuildPlot::~BuildPlot() {}

string BuildPlot::BuildSentence() {
	return TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is building " + _targetEdification->GetOwner()->GetActorLabel()));
}

void BuildPlot::BuildGraph() {

	_plotGraph = new Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph->AddNode(askForHelpNode);

	//GET TOOLS
	/*Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetArquetypeObject("martillo")
	_plotGraph->AddNode(getNode);*/

	//GATHER RESOURCES
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::gather);
	//gather what?
	_plotGraph->AddNode(getNode);

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetEdification->GetOwner()->GetActorLocation());
	_plotGraph->AddNode(goToNode);

	//BUILD
	Node* buildNode = new Node();
	buildNode->SetNodeType(NodeType::build);
	buildNode->SetEdification(_targetEdification);
	_plotGraph->AddNode(buildNode);
}

void BuildPlot::ConsiderReactions() {
}


//GIVE PLOT
//**************************************************************************************

GivePlot::GivePlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what) : BasePlot(plotEntity) {

	_targetEntity = target;
	_targetOwnable = what;
	_sentence = BuildSentence();
	_isExclusive = true;

	BuildGraph();
}

GivePlot::~GivePlot() {}

string GivePlot::BuildSentence() {
	return TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is giving " + _targetOwnable->GetOwner()->GetActorLabel() + " to " + _targetEntity->GetOwner()->GetActorLabel()));
}

void GivePlot::BuildGraph() {

	_plotGraph = new Graph();

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetEntity->GetOwner()->GetActorLocation());
	_plotGraph->AddNode(goToNode);

	//GIVE
	Node* giveNode = new Node();
	giveNode->SetNodeType(NodeType::give);
	giveNode->SetOwnable(_targetOwnable);
	_plotGraph->AddNode(giveNode);
}

void GivePlot::ConsiderReactions() {
}


