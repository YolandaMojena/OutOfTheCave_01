// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "StringCollection.h"
#include "Ontology/OEntity.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "Ontology/OEdification.h"
#include "PlotTypes.h"


//ATTACK PLOT
//**************************************************************************************

AttackPlot::AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = false;
	_motivation = motivation;
}

AttackPlot::~AttackPlot() {}

void AttackPlot::BuildSentence() {

	_sentence += _identifier + "\n";

	_sentence += "The brave, yet aggressive, " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
		+ " has began an attack against the despicable " + _targetEntity->GetRaceString() + " "
		+ _targetEntity->GetName() + ", who ";
		_sentence += _motivation->IsA<UOEntity>() ?
		"hurt his friend " + _motivation->GetOwner()->FindComponentByClass<UOEntity>()->GetName()
		: "damaged his " + _motivation->GetOwner()->GetActorLabel();

		_sentence += ". He counts with the help of ";

	if (_involvedInPlot.size() > 0) {
		for (int i = 0; i < _involvedInPlot.size(); i++) {
			_sentence += _involvedInPlot[i]->GetName();
			if (i <= _involvedInPlot.size() - 1)
				_sentence += ", ";
		}
		_sentence += " and ";
	}
	_sentence += "some allies.\n\n\n";
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
	attackNode->SetHighPriority(true);
	attackNode->SetEntityA(_targetEntity);
	_plotGraph.AddNode(attackNode);
}

void AttackPlot::InitPlot() {

	_identifier = "Attack " + _targetEntity->GetName() + ":\n";
	BuildGraph();
}

void AttackPlot::ConsiderReactions() {
}

UOEntity* AttackPlot::GetTargetEntity() {
	return _targetEntity;
}


//DESTROY PLOT
//**************************************************************************************

DestroyPlot::DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = false;
	_motivation = motivation;
}

DestroyPlot::~DestroyPlot() {}

void DestroyPlot::BuildSentence() {

	_sentence += _identifier + "\n";

	_sentence += "The brave, yet aggressive, " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
		+ " has began to destroy the " + _targetEdification->GetOwner()->GetActorLabel()  + " "
		", which belongs to ";

	if (_targetEdification->GetOwners().size() > 0) {
		for (int i = 0; i <  _targetEdification->GetOwners().size(); i++) {
			_sentence += _targetEdification->GetOwners()[i]->GetName();
			if (i <= _targetEdification->GetOwners().size() - 1)
				_sentence += ", ";
			else _sentence += " and ";
		}
	}
	_sentence += ", since " + _targetEntity->GetName();
	_sentence += _motivation->IsA<UOEntity>() ?
		" hurt his friend" + _motivation->GetOwner()->FindComponentByClass<UOEntity>()->GetName()
		: " damaged his " + _motivation->GetOwner()->GetActorLabel();

	_sentence += " He counts with the help of ";

	if (_involvedInPlot.size() > 0) {
		for (int i = 0; i < _involvedInPlot.size(); i++) {
			_sentence += _involvedInPlot[i]->GetName();
			if (i <= _involvedInPlot.size() - 1)
				_sentence += ", ";
		}
		_sentence += " and ";
	}
	_sentence += "some allies. .\n\n";
}

void DestroyPlot::BuildGraph() {

	_plotGraph = Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET TOOLS
	/*Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	_plotGraph->AddNode(getNode);*/

	//ASK TROLL FOR HELP
	/*Node* askTrollForHelpNode = new Node();
	askTrollForHelpNode->SetNodeType(NodeType::askTroll);
	_plotGraph.AddNode(askTrollForHelpNode);*/

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActorA(_targetEdification->GetOwner());
	_plotGraph.AddNode(goToNode);

	//DESTROY
	Node* destroyNode = new Node();
	destroyNode->SetNodeType(NodeType::destroy);
	destroyNode->SetEdification(_targetEdification);
	_plotGraph.AddNode(destroyNode);
}

void DestroyPlot::InitPlot() {

	for (OOwnership* o : _targetEntity->GetPossessions()) {

		if (dynamic_cast<UOEdification*>(o->GetOwnable())) {
			_targetEdification = (UOEdification*) o->GetOwnable();
			break;
		}
	}

	_identifier = "Destroy " + _targetEdification->GetOwner()->GetActorLabel() + "\n";
	BuildGraph();
}

void DestroyPlot::ConsiderReactions() {
}

UOEntity* DestroyPlot::GetTargetEntity() {
	return _targetEntity;
}


//STAMPEDE
//*************************************************************************************

Stampede::Stampede(ERace race, FVector spawnLocation, FVector targetLocation, float num, APlotGenerator* plotGenerator)  {

	_race = race;
	_spawnLocation = spawnLocation;
	_targetLocation = targetLocation;
	_plotGenerator = plotGenerator;
	_num = num;
}

Stampede::Stampede(ERace race, FVector spawnLocation, UOEntity* targetActor, float num, APlotGenerator* plotGenerator) {

	_race = race;
	_spawnLocation = spawnLocation;
	_targetActor = targetActor;
	_plotGenerator = plotGenerator;
	_num = num;
}

Stampede::~Stampede() {}

void Stampede::BuildSentence() {

	_sentence = "A stampede is approaching!";
}

void Stampede::BuildGraph() {

	_plotGraph = Graph();

	//GO TO LOCATION
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActorA(_targetActor->GetOwner());
	_plotGraph.AddNode(goToNode);

	//RETURN TO LOCATION
	/*goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_spawnLocation);
	_plotGraph.AddNode(goToNode);*/
}

void Stampede::InitPlot() {

	_identifier = "Stampede plot\n";
	BuildGraph();

	_heard = _plotGenerator->SpawnEntities(_num, _race);

	for (int i = 0; i < _heard.size(); i++) {

		_heard[i]->SetIdleGraph(&_plotGraph);
		_heard[i]->SetState(UOEntity::State::idle);
	}

	BuildSentence();
}

void Stampede::ConsiderReactions() {}


//BUILD PLOT
//**************************************************************************************

BuildPlot::BuildPlot(UOEntity* plotEntity, UOEdification* target, UItem* motivation) : BasePlot(plotEntity) {

	_targetEdification = target;
	_isExclusive = false;
	_motivation = motivation;
}

BuildPlot::~BuildPlot() {}

void BuildPlot::BuildSentence() {
	_sentence = "The unfortunate " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName() + " is rebuilding his/her home, which was destroyed by the merciless " + ((UOEntity*)_motivation)->GetRaceString() + " " + _motivation->GetName();
	_sentence += ". He counts with the help of ";

	if (_involvedInPlot.size() > 0) {
		for (int i = 0; i < _involvedInPlot.size(); i++) {
			_sentence += _involvedInPlot[i]->GetName();
			if (i <= _involvedInPlot.size() - 1)
				_sentence += ", ";
		}
		_sentence += " and ";
	}
	_sentence += "some allies.\n\n\n";
}

void BuildPlot::BuildGraph() {

	//_plotGraph = new Graph();

	/*//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET TOOLS
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetArquetypeObject("martillo")
	_plotGraph->AddNode(getNode);*/
	/*
	//GATHER RESOURCES
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::gather);
	//gather what?
	_plotGraph.AddNode(getNode);*/

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActorA(_targetEdification->GetOwner());
	_plotGraph.AddNode(goToNode);

	//BUILD
	Node* buildNode = new Node();
	buildNode->SetNodeType(NodeType::build);
	buildNode->SetEdification(_targetEdification);
	_plotGraph.AddNode(buildNode);
}

void BuildPlot::InitPlot() {

	_identifier = "Rebuild " + _plotEntity->GetName() + "'s home";
	BuildGraph();

}

void BuildPlot::ConsiderReactions() {}
	


//GATHER PLOT
//**************************************************************************************
/*
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
*/


//STEAL PLOT
//**************************************************************************************
/*
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
*/

/*
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
}*/


