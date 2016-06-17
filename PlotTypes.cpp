// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "StringCollection.h"
#include "Ontology/OEntity.h"
#include "Ontology/OOwnable.h"
#include "Ontology/OEdification.h"
#include "PlotTypes.h"


//ATTACK PLOT
//**************************************************************************************

AttackPlot::AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = false;
	_motivation = motivation;

	BuildGraph();
}

AttackPlot::~AttackPlot() {}

void AttackPlot::BuildSentence() {

	_sentence += "The brave " + _plotEntity->GetRace() + " " + _plotEntity->GetEntityName()
		+ " has began an attack against the despicable " + _targetEntity->GetRace() + " "
		+ _targetEntity->GetEntityName() + ", who " + _targetEntity->GetRace();
		_sentence += _motivation->IsA<UOEntity>() ?
		" hurt his friend" + _motivation->GetOwner()->FindComponentByClass<UOEntity>()->GetEntityName()
		: " damaged his " + _motivation->GetOwner()->GetActorLabel();

		_sentence += " He counts with the help of ";

	if (_involvedInPlot.size() > 0) {
		for (int i = 0; i < _involvedInPlot.size(); i++) {
			_sentence += _involvedInPlot[i]->GetEntityName();
			if (i <= _involvedInPlot.size() - 1)
				_sentence += ", ";
		}
		_sentence += " and ";
	}
	_sentence += "some allies. .\n\n";
}

void AttackPlot::BuildGraph() {

	_plotGraph = Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET WEAPON
	/*Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	n->SetArquetypeObject("espada")
	_plotGraph->AddNode(getNode);*/

	//ASK TROLL FOR HELP
	/*Node* askTrollForHelpNode = new Node();
	askTrollForHelpNode->SetNodeType(NodeType::askTroll);
	_plotGraph.AddNode(askTrollForHelpNode);*/

	//GO TO KILLER
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActorA(_targetEntity->GetOwner());
	_plotGraph.AddNode(goToNode);

	//ATTACK
	Node* attackNode = new Node();
	attackNode->SetNodeType(NodeType::attack);
	attackNode->SetEntityA(_targetEntity);
	_plotGraph.AddNode(attackNode);
}


void AttackPlot::ConsiderReactions() {
}


//GATHER PLOT
//**************************************************************************************

GatherPlot::GatherPlot(UOEntity* plotEntity, UOOwnable* targetResource) : BasePlot(plotEntity) {

	_targetResource = targetResource;
	_isExclusive = false;

	BuildGraph();
}

GatherPlot::~GatherPlot() {}

void GatherPlot::BuildSentence() {
	_sentence = TCHAR_TO_UTF8(*("Entity " + _plotEntity->GetOwner()->GetActorLabel() + " is gathering " + _targetResource->GetOwner()->GetActorLabel()));
}

void GatherPlot::BuildGraph() {

}

void GatherPlot::ConsiderReactions() {
}


//DESTROY PLOT
//**************************************************************************************

DestroyPlot::DestroyPlot(UOEntity* plotEntity, UOOwnable* target) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_isExclusive = false;

	BuildGraph();
}

DestroyPlot::~DestroyPlot() {}

void DestroyPlot::BuildSentence() {
	_sentence = TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is destroying " + _targetOwnable->GetOwner()->GetActorLabel()));
}

void DestroyPlot::BuildGraph() {

	//_plotGraph = new Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET TOOLS
	/*Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	_plotGraph->AddNode(getNode);*/

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetOwnable->GetOwner()->GetActorLocation());
	_plotGraph.AddNode(goToNode);

	//DESTROY
	Node* destroyNode = new Node();
	destroyNode->SetNodeType(NodeType::destroy);
	destroyNode->SetOwnable(_targetOwnable);
	_plotGraph.AddNode(destroyNode);
}


void DestroyPlot::ConsiderReactions() {
}


//STEAL PLOT
//**************************************************************************************

StealPlot::StealPlot(UOEntity* plotEntity, UOEntity* who, UOOwnable* target) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_targetEntity = who;
	_isExclusive = true;

	BuildGraph();
}

StealPlot::~StealPlot() {}

void StealPlot::BuildSentence() {
	_sentence = TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is stealing " + _targetOwnable->GetOwner()->GetActorLabel() + " " + _targetEntity->GetOwner()->GetActorLabel()));
}

void StealPlot::BuildGraph() {

	//_plotGraph = new Graph();

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetEntity->GetOwner()->GetActorLocation());
	_plotGraph.AddNode(goToNode);

	//STEAL
	Node* stealNode = new Node();
	stealNode->SetNodeType(NodeType::steal);
	stealNode->SetEntityA(_targetEntity);
	stealNode->SetOwnable(_targetOwnable);
	_plotGraph.AddNode(stealNode);
}

void StealPlot::ConsiderReactions() {
}


//BUILD PLOT
//**************************************************************************************

BuildPlot::BuildPlot(UOEntity* plotEntity, UOEdification* target) : BasePlot(plotEntity) {

	_targetEdification = target;
	_isExclusive = false;

	BuildGraph();
}

BuildPlot::~BuildPlot() {}

void BuildPlot::BuildSentence() {
	_sentence = TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is building " + _targetEdification->GetOwner()->GetActorLabel()));
}

void BuildPlot::BuildGraph() {

	//_plotGraph = new Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET TOOLS
	/*Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetArquetypeObject("martillo")
	_plotGraph->AddNode(getNode);*/

	//GATHER RESOURCES
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::gather);
	//gather what?
	_plotGraph.AddNode(getNode);

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetEdification->GetOwner()->GetActorLocation());
	_plotGraph.AddNode(goToNode);

	//BUILD
	Node* buildNode = new Node();
	buildNode->SetNodeType(NodeType::build);
	buildNode->SetEdification(_targetEdification);
	_plotGraph.AddNode(buildNode);
}

void BuildPlot::ConsiderReactions() {
}


//GIVE PLOT
//**************************************************************************************

GivePlot::GivePlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what) : BasePlot(plotEntity) {

	_targetEntity = target;
	_targetOwnable = what;
	_isExclusive = true;

	BuildGraph();
}

GivePlot::~GivePlot() {}

void GivePlot::BuildSentence() {
	_sentence = TCHAR_TO_UTF8(*("Entity: " + _plotEntity->GetOwner()->GetActorLabel() + " is giving " + _targetOwnable->GetOwner()->GetActorLabel() + " to " + _targetEntity->GetOwner()->GetActorLabel()));
}

void GivePlot::BuildGraph() {

	//_plotGraph = new Graph();

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetEntity->GetOwner()->GetActorLocation());
	_plotGraph.AddNode(goToNode);

	//GIVE
	Node* giveNode = new Node();
	giveNode->SetNodeType(NodeType::give);
	giveNode->SetOwnable(_targetOwnable);
	_plotGraph.AddNode(giveNode);
}

void GivePlot::ConsiderReactions() {
}


