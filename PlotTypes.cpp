// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "StringCollection.h"
#include "Ontology/OEntity.h"
#include "Ontology/OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "NarrativeGeneration/Ambition.h"
#include "Ontology/OEdification.h"
#include "PlotTypes.h"



//ATTACK PLOT
//**************************************************************************************

AttackPlot::AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = false;
	_motivation = motivation;
}

AttackPlot::AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_ambition = ambition;
}

AttackPlot::~AttackPlot() {}

void AttackPlot::BuildSentence() {

	_sentence += _identifier + "\n";

	if(_motivation){

		_sentence += "The brave " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
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
		_sentence += "some allies.\n\n";
	}
	else if (_ambition == TypeOfAmbition::extermination) {

		_sentence += "The aggressive " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " has began an attack against the unfortunate " + _targetEntity->GetRaceString() + " "
			+ _targetEntity->GetName() + ", in order to exterminate his race.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::extermination) {

		_sentence += "The aggressive " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " has began an attack against the highly notorious " + _targetEntity->GetRaceString() + " "
			+ _targetEntity->GetName() + ", in order to obtain his/her position.\n\n\n";
	}
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

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();

	if (_plotEntity->GetRelationWith(troll)->GetAppreciation() > 50) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::goToItem);
		askTrollForHelpNode->SetActorA(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

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

DestroyPlot::DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_ambition = ambition;
}

DestroyPlot::~DestroyPlot() {}

void DestroyPlot::BuildSentence() {

	_sentence += _identifier + "\n";

	if (_motivation) {
		_sentence += "The brave " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " has began to destroy the " + _targetEdification->GetOwner()->GetActorLabel() + " "
			", which belongs to ";

		if (_targetEdification->GetOwners().size() > 0) {
			for (int i = 0; i < _targetEdification->GetOwners().size(); i++) {
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

	else {

	}

	
}

void DestroyPlot::BuildGraph() {

	_plotGraph = Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();
	if (_plotEntity->GetRelationWith(troll)->GetAppreciation() > 50) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::goToItem);
		askTrollForHelpNode->SetActorA(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

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
	_sentence += "some allies.\n\n";
}

void BuildPlot::BuildGraph() {

	_plotGraph = Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET TOOLS
	/*Node* getNode = new Node();
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

UOEdification* BuildPlot::GetTargetEdification() {
	return _targetEdification;
}
	

// AMBUSH
// ********************************************************************
AmbushPlot::AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_motivation = motivation;
}

AmbushPlot::AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_ambition = ambition;
}

AmbushPlot::~AmbushPlot() {}

void AmbushPlot::BuildSentence() {

	_sentence += _identifier + "\n";

	if (_motivation) {

		_sentence += "The unfaithful " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " has commited traison and organized an ambush against the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetName()
			+ ", who planned to attack his/her friend, the " + ((UOEntity*)_motivation)->GetRaceString() + " " + _motivation->GetName();

		_sentence += ". He counts with the help of ";

		if (_involvedInPlot.size() > 0) {
			for (int i = 0; i < _involvedInPlot.size(); i++) {
				_sentence += _involvedInPlot[i]->GetName();
				if (i <= _involvedInPlot.size() - 1)
					_sentence += ", ";
			}
			_sentence += " and ";
		}
		_sentence += "some allies.\n\n";
	}
	else if (_ambition == TypeOfAmbition::extermination) {

		_sentence += "The astute " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " is assaulting " + _targetEntity->GetRaceString() + "s! "
			+ "This time his/her victim has been the unfortunate" + _targetEntity->GetName() +
			". He/she will go as far a neccesary to exterminate the race.\n\n";
	}
}

void AmbushPlot::BuildGraph() {

	_plotGraph = Graph();

	//GET WEAPON
	/*Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	n->SetArquetypeObject("espada")
	_plotGraph->AddNode(getNode);*/

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();

	if (_plotEntity->GetRelationWith(troll)->GetAppreciation() > 50 && _motivation) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::goToItem);
		askTrollForHelpNode->SetActorA(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	//GO TO STRATEGIC LOCATION NEAR TO VICTIM
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goTo);
	goToNode->SetPosition(_targetEntity->GetOwner()->GetActorLocation() - FVector(600, 600, 0));
	_plotGraph.AddNode(goToNode);

	Node* waitNode = new Node();
	waitNode->SetNodeType(NodeType::wait);
	waitNode->SetEntityA(_targetEntity);
	_plotGraph.AddNode(waitNode );

	//ATTACK
	Node* attackNode = new Node();
	attackNode->SetNodeType(NodeType::attack);
	attackNode->SetHighPriority(true);
	attackNode->SetEntityA(_targetEntity);
	_plotGraph.AddNode(attackNode);
}

void AmbushPlot::InitPlot() {

	_identifier = "Ambush against " + _targetEntity->GetName() + ":\n";
	BuildGraph();
}

void AmbushPlot::ConsiderReactions() {
}

UOEntity* AmbushPlot::GetTargetEntity() {
	return _targetEntity;
}


//STEAL PLOT
//**************************************************************************************

StealPlot::StealPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOOwnable* target, UItem* motivation) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_targetEntity = targetEntity;
	_motivation = motivation;
	_isExclusive = true;
}

StealPlot::StealPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOOwnable* target, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_targetEntity = targetEntity;
	_ambition = ambition;
	_isExclusive = true;
}

StealPlot::~StealPlot() {}

void StealPlot::BuildSentence() {

	if (_motivation) {
		_sentence = "The vengeful " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " is stealing a " + _targetOwnable->GetName() + " from the " + _targetEntity->GetRaceString() +
			" " + _targetEntity->GetName() + ", since it was also stolen from him/her.\n\n";
	}
	else if (_ambition == TypeOfAmbition::possessions) {
		_sentence = "The materialistic " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " is stealing a precious " + _targetOwnable->GetName() + " from the " + _targetEntity->GetRaceString() +
			" " + _targetEntity->GetName() + ", since his/her ambition in life is to own lots of valuable possessions.\n\n";
	}
}

void StealPlot::BuildGraph() {

	_plotGraph = Graph();

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

	//If he's a mad coward, kill
	if (_plotEntity->GetPersonality()->GetAggressiveness() > 85 && _plotEntity->GetPersonality()->GetBraveness() < 15) {
		Node* attackNode = new Node();
		attackNode->SetNodeType(NodeType::attack);
		attackNode->SetEntityA(_targetEntity);
		_plotGraph.AddNode(attackNode);
	}
}

void StealPlot::InitPlot() {

	_identifier = "Steal " + _targetEntity->GetName() + "'s " + _targetOwnable->GetName() + ": \n";
	BuildGraph();
}

void StealPlot::ConsiderReactions() {
}

UOEntity* StealPlot::GetTargetEntity() {
	return _targetEntity;
}

UOOwnable* StealPlot::GetTargetOwnable() {
	return _targetOwnable;
}



//GET PLOT
//**************************************************************************************

GetPlot::GetPlot(UOEntity* plotEntity, UOOwnable* target, UItem* motivation) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_motivation = motivation;
	_isExclusive = true;
}

GetPlot::GetPlot(UOEntity* plotEntity, UOOwnable* target, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_ambition = ambition;
	_isExclusive = true;
}

GetPlot::~GetPlot() {}

void GetPlot::BuildSentence() {

	if (_motivation) {
		_sentence = "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " is in need of a" + _targetOwnable->GetName()
			+ ". As he/she doesn't own one/enough, he/she is trasversing the land to get it.\n\n";
	}
	else if (_ambition == TypeOfAmbition::possessions) {
		_sentence = "The materialistic " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " is embarking on an adventure to find a precious " + _targetOwnable->GetName() +
			", since his/her ambition in life is to own lots of valuable possessions.\n\n";
	}
}

void GetPlot::BuildGraph() {

	_plotGraph = Graph();

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();
	if (_plotEntity->GetRelationWith(troll)->GetAppreciation() > 50) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::goToItem);
		askTrollForHelpNode->SetActorA(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	//GO TO TARGET
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetOwnable(_targetOwnable);
	_plotGraph.AddNode(getNode);
}

void GetPlot::InitPlot() {

	_identifier = "Get " + _targetOwnable->GetName() + "from world.\n";
	BuildGraph();
}

void GetPlot::ConsiderReactions() {
}

UOOwnable* GetPlot::GetTargetOwnable() {
	return _targetOwnable;
}


// HELP / FOLLOW AROUND
// ********************************************************************
HelpPlot::HelpPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_motivation = motivation;
}

HelpPlot::HelpPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_ambition = ambition;
}

HelpPlot::~HelpPlot() {}

void HelpPlot::BuildSentence() {

	_sentence += _identifier + "\n";

	if (_motivation) {
	}

	else if (_ambition == TypeOfAmbition::notoriety) {

		_sentence += "The kind " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " is doing his/her best to help the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetName()
			+ ", since he/she is highly notorious in society. Maybe one day he/she'll get to be like him/her.\n\n";
	}
	else if (_ambition == TypeOfAmbition::friendTroll) {

		_sentence += "The curious " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ "wants to be the troll's friend! He/She's following him around to learn from him and "
			+ "maybe, someday, do something useful for him\n\n.";
	}
}

void HelpPlot::BuildGraph() {

	_plotGraph = Graph();

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActorA(_targetEntity->GetOwner());
	_plotGraph.AddNode(goToNode);

	// HELP
	Node* helpNode = new Node();
	helpNode->SetNodeType(NodeType::help);
	helpNode->SetEntityA(_targetEntity);
	_plotGraph.AddNode(helpNode);

}

void HelpPlot::InitPlot() {

	_identifier = "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName() +
		"wants to help!\n";

	BuildGraph();
}

void HelpPlot::ConsiderReactions() {
}

UOEntity* HelpPlot::GetTargetEntity() {
	return _targetEntity;
}


//STAMPEDE
//*************************************************************************************

Stampede::Stampede(ERace race, FVector spawnLocation, FVector targetLocation, float num, APlotGenerator* plotGenerator) {

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


