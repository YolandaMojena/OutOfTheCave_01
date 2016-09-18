// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
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
	_ambition = TypeOfAmbition::noAmbition;
}

AttackPlot::AttackPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_ambition = ambition;
	_motivation = nullptr;
}

AttackPlot::~AttackPlot() {}

void AttackPlot::BuildSentence() {

	_sentence += _identifier;

	if(_motivation){

		_sentence += "The brave " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName() 
			+ " began an attack against the despicable " + _targetEntity->GetRaceString()
			+ _targetEntity->GetItemName() + ", who had ";
		_sentence += _motivation->IsA<UOEntity>() ?
			" hurt his/her friend " + _motivation->GetItemName()
			: " damaged his/her " + _motivation->GetItemName();

		_sentence += ".\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::extermination) {

		_sentence += "The aggressive " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " began an attack against the unfortunate " + _targetEntity->GetRaceString() + " "
			+ _targetEntity->GetItemName() + ", in order to exterminate his/her race.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::notoriety) {

		_sentence += "The aggressive " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " has begun an attack against the highly notorious " + _targetEntity->GetRaceString() + " "
			+ _targetEntity->GetItemName() + ", in order to obtain his/her position in society.\n\n\n";
	}
}

void AttackPlot::BuildGraph() {

	_plotGraph = Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET WEAPON
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetAffordableUse(OntologicFunctions::AffordableUse::build);
	_plotGraph.AddNode(getNode);

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();
	ORelation* relation = _plotEntity->GetRelationWith(troll);
	if (relation && relation->GetAppreciation() > 50) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::askTroll);
		askTrollForHelpNode->SetActor(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	//ATTACK
	Node* attackNode = new Node();
	attackNode->SetNodeType(NodeType::attack);
	attackNode->SetHighPriority(true);
	attackNode->SetActor(_targetEntity->GetOwner());
	attackNode->SetEntity(_targetEntity);
	_plotGraph.AddNode(attackNode);
}

void AttackPlot::InitPlot() {

	_identifier = "Attack " + _targetEntity->GetItemName() + ":\n";
	BuildGraph();
	BuildSentence();
}

BasePlot* AttackPlot::ConsiderReactions() {

	BasePlot* reaction = nullptr;
	UOEntity* reactionEntity = nullptr;

	for (UOEntity* e : _involvedInPlot) {

		if(e->GetPersonality()->GetAstute()>50){

			ORelation* relationWithMain = e->GetRelationWith(_plotEntity);
			ORelation* relationWithOther = e->GetRelationWith(_targetEntity);

			// SHOULD I VALUE RESPECT AND FEAR? 
			if (relationWithMain && relationWithOther && 
				(relationWithOther->GetAppreciation()  + relationWithOther->GetRespect() + relationWithOther->GetFear()) > 
				(relationWithMain->GetAppreciation() + relationWithMain->GetRespect() + relationWithMain->GetFear())) {

				DeleteFromInvolved(e);

				if (!reactionEntity) {
					reactionEntity = e;

					/*if (reactionEntity->GetPersonality()->GetAggressiveness() > 50)
						reaction = new AmbushPlot(e, _plotEntity, _targetEntity);
					else*/
						reaction = new DefendPlot(e, _plotEntity, _targetEntity);
				}
				else if (reaction)
					reaction->AddInvolvedInPlot(e);
			}
		}
	}	
		
	return reaction;
}

UOEntity* AttackPlot::GetTargetEntity() {
	return _targetEntity;
}


//DESTROY PLOT
//**************************************************************************************

DestroyPlot::DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOEdification* targetEdification, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = false;
	_targetEdification = targetEdification;
	_motivation = motivation;
	_ambition = TypeOfAmbition::noAmbition;
}

DestroyPlot::DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOEdification* targetEdification, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_targetEdification = targetEdification;
	_ambition = ambition;
	_motivation = nullptr;
}

DestroyPlot::~DestroyPlot() {}

void DestroyPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {
		_sentence += "The brave " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " began to destroy the " + _targetEdification->GetItemName() +
			", which belongs to ";

		if (_targetEdification->GetOwners().size() > 0) {
			for (int i = 0; i < _targetEdification->GetOwners().size(); i++) {
				_sentence += _targetEdification->GetOwners()[i]->GetItemName();
				if (i < _targetEdification->GetOwners().size() - 2)
					_sentence += ", ";
				else _sentence += " and ";
			}
		}
		_sentence += ", since " + _targetEntity->GetItemName();
		_sentence += _motivation->IsA<UOEntity>() ?
			" had hurt his/her friend " + _motivation->GetItemName()
			: " had damaged his/her " + _motivation->GetItemName();

		_sentence += ".\n\n\n";
	}

	else if (_ambition == TypeOfAmbition::fear) {

		_sentence += "The coward " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " began to destroy a " + ((UOEntity*)(_targetEdification->GetOwners()[0]))->GetRaceString() + "'s home.";

		_sentence += " His/Her hope was that this would lead others to fear him.\n\n\n";
	}
}

void DestroyPlot::BuildGraph() {

	_plotGraph = Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET TOOLS
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetAffordableUse(OntologicFunctions::weapon);
	_plotGraph.AddNode(getNode);

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();
	ORelation* relation = _plotEntity->GetRelationWith(troll);
	if (relation && relation->GetAppreciation() > 50) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::askTroll);
		askTrollForHelpNode->SetActor(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	//DESTROY
	Node* destroyNode = new Node();
	destroyNode->SetNodeType(NodeType::destroy);
	destroyNode->SetActor(_targetEdification->GetOwner());
	destroyNode->SetEdification(_targetEdification);
	_plotGraph.AddNode(destroyNode);
}

void DestroyPlot::InitPlot() {

	_identifier = "Destroy " + _targetEdification->GetItemName() + ":\n";
	BuildGraph();
	BuildSentence();
}

BasePlot* DestroyPlot::ConsiderReactions() {

	BasePlot* reaction = nullptr;
	UOEntity* reactionEntity = nullptr;

	for (UOEntity* e : _involvedInPlot) {

		if (e->GetPersonality()->GetAstute()>50) {

			ORelation* relationWithMain = e->GetRelationWith(_plotEntity);
			ORelation* relationWithOther = e->GetRelationWith(_targetEntity);

			// SHOULD I VALUE RESPECT AND FEAR? 
			if (relationWithMain && relationWithOther &&
				(relationWithOther->GetAppreciation() + relationWithOther->GetRespect() + relationWithOther->GetFear()) >
				(relationWithMain->GetAppreciation() + relationWithMain->GetRespect() + relationWithMain->GetFear())) {

				DeleteFromInvolved(e);

				if (!reactionEntity) {
					reactionEntity = e;

					/*if (reactionEntity->GetPersonality()->GetAggressiveness() > 50)
						reaction = new AmbushPlot(e, _plotEntity, _targetEdification);
					else*/
						reaction = new DefendPlot(e, _plotEntity, _targetEdification);
				}
				else if (reaction)
					reaction->AddInvolvedInPlot(e);
			}
		}
	}
	return reaction;
}

UOEntity* DestroyPlot::GetTargetEntity() {
	return _targetEntity;
}

UOEdification * DestroyPlot::GetTargetEdification()
{
	return _targetEdification;
}


//BUILD PLOT
//**************************************************************************************

BuildPlot::BuildPlot(UOEntity* plotEntity, UOEdification* target, UItem* motivation) : BasePlot(plotEntity) {

	_targetEdification = target;
	_isExclusive = false;
	_motivation = motivation;
	_ambition = TypeOfAmbition::noAmbition;
}

BuildPlot::BuildPlot(UOEntity* plotEntity, UOEdification* target, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEdification = target;
	_isExclusive = false;
	_motivation = nullptr;
	_ambition = ambition;
}

BuildPlot::~BuildPlot() {}

void BuildPlot::BuildSentence() {

	_sentence += _identifier;
	_sentence += "The unfortunate " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName() + " had to rebuild his/her home, which had been destroyed by the merciless " + ((UOEntity*)_motivation)->GetRaceString() + " " + _motivation->GetItemName();
	_sentence += ".\n\n\n";
}

void BuildPlot::BuildGraph() {

	_plotGraph = Graph();

	//ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET TOOLS
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetAffordableUse(OntologicFunctions::mine);
	_plotGraph.AddNode(getNode);

	//BUILD
	Node* buildNode = new Node();
	buildNode->SetNodeType(NodeType::build);
	buildNode->SetActor(_targetEdification->GetOwner());
	buildNode->SetEdification(_targetEdification);
	_plotGraph.AddNode(buildNode);
}

void BuildPlot::InitPlot() {

	_identifier = "Rebuild " + _plotEntity->GetItemName() + "'s home:\n";
	BuildGraph();
	BuildSentence();

}

BasePlot* BuildPlot::ConsiderReactions() {
	return nullptr;
}

UOEdification* BuildPlot::GetTargetEdification() {
	return _targetEdification;
}
	

// AMBUSH
// ********************************************************************
AmbushPlot::AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_motivation = motivation;
	_ambition = TypeOfAmbition::noAmbition;
}

AmbushPlot::AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_ambition = ambition;
	_motivation = nullptr;
}

AmbushPlot::~AmbushPlot() {}

void AmbushPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {

		_sentence += "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " committed an act of treason and organized an ambush against the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetItemName()
			+ ", who had planned to ";
		_sentence += _target->IsA<UOEntity>() ? " attack his/her friend " + _motivation->GetItemName() : " destroy his/her friend's " + _motivation->GetItemName();
		_sentence += ".\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::extermination) {

		_sentence += "The astute " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " assaulted the " + _targetEntity->GetRaceString() + "s! "
			+ "This time his/her victim was the unfortunate" + _targetEntity->GetItemName() +
			". He/she will go as far a necessary to exterminate the race.\n\n\n";
	}
}

void AmbushPlot::BuildGraph() {

	_plotGraph = Graph();

	// ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetHighPriority(true);
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET WEAPON
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetAffordableUse(OntologicFunctions::weapon);
	_plotGraph.AddNode(getNode);

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();
	ORelation* relation = _plotEntity->GetRelationWith(troll);
	if (relation && relation->GetAppreciation() > 50) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::askTroll);
		askTrollForHelpNode->SetActor(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	Node* ambushNode = new Node();
	ambushNode->SetNodeType(NodeType::ambush);
	ambushNode->SetPosition(_targetEntity->GetGoingLocation());
	ambushNode->SetEntity(_targetEntity);
	_plotGraph.AddNode(ambushNode);

	//ATTACK
	Node* attackNode = new Node();
	attackNode->SetNodeType(NodeType::attack);
	attackNode->SetHighPriority(true);
	attackNode->SetEntity(_targetEntity);
	_plotGraph.AddNode(attackNode);
}

void AmbushPlot::InitPlot() {

	_identifier = "Ambush against " + _targetEntity->GetItemName() + ":\n";
	BuildGraph();
	BuildSentence();
}

BasePlot* AmbushPlot::ConsiderReactions() {
	return nullptr;
}

UOEntity* AmbushPlot::GetTargetEntity() {
	return _targetEntity;
}


//GET PLOT
//**************************************************************************************

GetPlot::GetPlot(UOEntity* plotEntity, UOOwnable* target, UItem* motivation) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_motivation = motivation;
	_isExclusive = true;
	_ambition = TypeOfAmbition::noAmbition;
}

GetPlot::GetPlot(UOEntity* plotEntity, UOOwnable* target, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_ambition = ambition;
	_isExclusive = true;
	_motivation = nullptr;
}

GetPlot::~GetPlot() {}

void GetPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {
		_sentence += "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " was in need of a " + _targetOwnable->GetItemName()
			+ ". He/she traversed the land to get it.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::possessions) {
		_sentence += "The materialistic " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " embarked on an adventure to find a precious " + _targetOwnable->GetItemName() +
			", since his/her ambition in life is to own lots of valuable possessions.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::appreciation) {
		_sentence += "The materialistic " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " embarked on an adventure to find a precious " + _targetOwnable->GetItemName() +
			". It is for someone he/she wants to be noticed by.\n\n\n";
	}
}

void GetPlot::BuildGraph() {

	_plotGraph = Graph();

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();
	ORelation* relation = _plotEntity->GetRelationWith(troll);
	if (relation && relation->GetAppreciation() > 50){

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::goToActor);
		askTrollForHelpNode->SetActor(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	//GET OBJECT
	Node* grabNode = new Node();
	grabNode->SetNodeType(NodeType::grab);
	grabNode->SetOwnable(_targetOwnable);
	grabNode->SetActor(_targetOwnable->GetOwner());
	_plotGraph.AddNode(grabNode);
}

void GetPlot::InitPlot() {

	_identifier = "Get " + _targetOwnable->GetItemName() + " from world:\n";
	BuildGraph();
	BuildSentence();
}

BasePlot* GetPlot::ConsiderReactions() {
	return nullptr;
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
	_ambition = TypeOfAmbition::noAmbition;
}

HelpPlot::HelpPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_ambition = ambition;
	_motivation = nullptr;
}

HelpPlot::~HelpPlot() {}

void HelpPlot::BuildSentence() {

	_sentence += _identifier + "\n";

	if (_motivation) {
		
		_sentence += "The kind " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " did his/her best to help the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetItemName()
			+ ", since he/she was really helpful to him/her in the past.\n\n\n";
	}

	else if (_ambition == TypeOfAmbition::appreciation) {

		_sentence += "The kind " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " did his/her best to help the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetItemName()
			+ ", since he/she would like to be more appreciated by him/her.\n\n\n";
	}

	else if (_ambition == TypeOfAmbition::notoriety) {

		_sentence += "The kind " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " did his/her best to help the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetItemName()
			+ ", since he/she is highly notorious in society. Maybe one day he/she'll get to be like him/her.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::friendTroll) {

		_sentence += "The curious " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ "wants to be the troll's friend! He/She's started following him around to learn from him and "
			+ "maybe, someday, do something useful for him.\n\n\n.";
	}
}

void HelpPlot::BuildGraph() {

	_plotGraph = Graph();

	// HELP
	Node* helpNode = new Node();
	helpNode->SetNodeType(NodeType::help);
	helpNode->SetEntity(_targetEntity);
	helpNode->SetPosition(_targetEntity->GetOwner()->GetActorLocation());
	if (_targetEntity->IsPlayer) {
		helpNode->SetActor(_targetEntity->GetOwner());
		ORelation* trollRelation = _plotEntity->GetRelationWith(_targetEntity);
		if (!trollRelation)
			_plotEntity->AddRelationship(_targetEntity);
		trollRelation->ChangeAppreciation(+10);
		trollRelation->ChangeFear(-10);
	}
		
	_plotGraph.AddNode(helpNode);

}

void HelpPlot::InitPlot() {

	_identifier = "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName() +
		"wants to help!:\n";
	BuildGraph();
	BuildSentence();
}

BasePlot* HelpPlot::ConsiderReactions() {
	return nullptr;
}

UOEntity* HelpPlot::GetTargetEntity() {
	return _targetEntity;
}


//GIVE PLOT
//**************************************************************************************

GivePlot::GivePlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = target;
	_targetOwnable = what;
	_isExclusive = true;
	_motivation = motivation;
	_ambition = TypeOfAmbition::noAmbition;
}

GivePlot::GivePlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = target;
	_targetOwnable = what;
	_isExclusive = true;
	_ambition = ambition;
	_motivation = nullptr;
}

GivePlot::~GivePlot() {}

void GivePlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {

		_sentence += "The generous: " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName() + " is giving his precious " + _targetOwnable->GetItemName() + " to the " +
			_targetEntity->GetRaceString() + " " + _targetEntity->GetItemName() + ", in gratitude for the help received with his/her " + _motivation->GetItemName() + ".\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::appreciation) {

		_sentence += "The generous: " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName() + " is giving a precious " + _targetOwnable->GetItemName() + " to the " +
			_targetEntity->GetRaceString() + " " + _targetEntity->GetItemName() + ", since he/she feels he/she would like to be more appreciated by others.\n\n\n";
	}
}

void GivePlot::BuildGraph() {

	_plotGraph = Graph();

	//GET TARGET
	Node* grabNode = new Node();
	grabNode->SetNodeType(NodeType::grab);
	grabNode->SetOwnable(_targetOwnable);
	grabNode->SetActor(_targetOwnable->GetOwner());
	_plotGraph.AddNode(grabNode);

	//GIVE
	Node* giveNode = new Node();
	giveNode->SetNodeType(NodeType::give);
	giveNode->SetOwnable(_targetOwnable);
	giveNode->SetEntity(_targetEntity);
	_plotGraph.AddNode(giveNode);
}

void GivePlot::InitPlot() {

	_identifier = "Give " + _targetOwnable->GetItemName() + " to " + _targetEntity->GetItemName() + ":\n";

	BuildGraph();
	BuildSentence();
}

BasePlot* GivePlot::ConsiderReactions() {
	return nullptr;
}

UOEntity* GivePlot::GetTargetEntity() {
	return _targetEntity;
}

UOOwnable* GivePlot::GetTargetOwnable() {
	return _targetOwnable;
}

//STEAL PLOT
//**************************************************************************************

StealPlot::StealPlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = target;
	_targetOwnable = what;
	_isExclusive = true;
	_motivation = motivation;
	_ambition = TypeOfAmbition::noAmbition;
}

StealPlot::StealPlot(UOEntity* plotEntity, UOEntity* target, UOOwnable* what, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = target;
	_targetOwnable = what;
	_isExclusive = true;
	_ambition = ambition;
	_motivation = nullptr;
}

StealPlot::~StealPlot() {}

void StealPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {

		_sentence += "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName() + " decided to steal a " + _targetOwnable->GetItemName() + " from the " +
			_targetEntity->GetRaceString() + " " + _targetEntity->GetItemName() + " as an act of revenge for his/her own stolen " + _motivation->GetItemName() + ".\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::possessions) {
		_sentence += "The materialistic " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName()
			+ " stole a precious " + _targetOwnable->GetItemName() + " from the " + _targetEntity->GetRaceString() +
			" " + _targetEntity->GetItemName() + ", since his/her ambition in life is to own lots of valuable possessions.\n\n\n";
	}
}

void StealPlot::BuildGraph() {

	_plotGraph = Graph();

	//STEAL
	Node* stealNode = new Node();
	stealNode->SetNodeType(NodeType::steal);
	stealNode->SetOwnable(_targetOwnable);
	_plotGraph.AddNode(stealNode);

	//If he's a mad coward, kill
	if (_plotEntity->GetPersonality()->GetAggressiveness() > 85 && _plotEntity->GetPersonality()->GetBraveness() < 15) {
		Node* attackNode = new Node();
		attackNode->SetNodeType(NodeType::attack);
		attackNode->SetEntity(_targetEntity);
		_plotGraph.AddNode(attackNode);
	}
}

void StealPlot::InitPlot() {

	_identifier = "Steal " + _targetOwnable->GetItemName() + " from " + _targetEntity->GetItemName() + ":\n";

	BuildGraph();
	BuildSentence();
}

BasePlot* StealPlot::ConsiderReactions() {
	return nullptr;
}

UOEntity* StealPlot::GetTargetEntity() {
	return _targetEntity;
}

UOOwnable* StealPlot::GetTargetOwnable() {
	return _targetOwnable;
}

//DEFEND PLOT
//**************************************************************************************

DefendPlot::DefendPlot(UOEntity* plotEntity, UOEntity* against, UItem* motivation) : BasePlot(plotEntity) {

	_against = against;
	_motivation = motivation;
	_ambition = TypeOfAmbition::noAmbition;
	_isExclusive = false;
}

DefendPlot::~DefendPlot() {}

void DefendPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {

		_sentence += "And when everything seemed to be lost for the " + (_motivation->IsA<UOEntity>() ? ((UOEntity*)_motivation)->GetRaceString() : + "") + " " + _motivation->GetItemName() +
			", the astute " + _plotEntity->GetRaceString() + " " + _plotEntity->GetItemName() + " came up with a defensive strategy to protect " ;
		(_sentence += _motivation->IsA<UOEntity>() ? + "him/her " : + "it ") + " from the "
			+ _against->GetRaceString() + " " + _against->GetItemName();
			
		_sentence += ".\n\n\n";
			
	}
}

void DefendPlot::BuildGraph() {

	_plotGraph = Graph();

	// ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetHighPriority(true);
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//DEFEND
	Node* defendNode = new Node();
	defendNode->SetNodeType(NodeType::defend);
	defendNode->SetEntity(_against);
	defendNode->SetActor(_motivation->GetOwner());
	_plotGraph.AddNode(defendNode);
}

void DefendPlot::InitPlot() {

	_identifier = "Defend for " + ((UOEntity*)_motivation)->GetRaceString() + " " + ((UOEntity*)_motivation)->GetItemName() + ":\n";

	BuildGraph();
	BuildSentence();
}

BasePlot* DefendPlot::ConsiderReactions() {
	return nullptr;
}



//STAMPEDE
//*************************************************************************************

Stampede::Stampede(ERace race, FVector spawnLocation, float num, APlotGenerator* plotGenerator) {

	_race = race;
	_spawnLocation = spawnLocation + Utilities::RandomDisplacementVector(500);

	FVector dir;
	float dist;
	if (rand() % 10 < 5.0f) {
		dir = FVector(1, 0, 0);
		dist = 13000;
	}
	else {
		dir = FVector(0, -1, 0);
		dist = 11000;
	}
	_targetLocation = _spawnLocation + dir*dist;
	//_targetLocation = _spawnLocation + FVector(UGameplayStatics::GetPlayerCharacter(plotGenerator->GetWorld(), 0)->GetActorLocation() - _spawnLocation).GetSafeNormal() * FVector(5000, 5000, 0);

	_plotGenerator = plotGenerator;
	_num = num;
	_targetActor = nullptr;

	_motivation = nullptr;
	_ambition = TypeOfAmbition::noAmbition;
}

Stampede::Stampede(ERace race, FVector spawnLocation, UOEntity* targetActor, float num, APlotGenerator* plotGenerator) {

	_race = race;
	_spawnLocation = spawnLocation;
	_targetActor = targetActor;
	_plotGenerator = plotGenerator;
	_num = num;

	_motivation = nullptr;
	_ambition = TypeOfAmbition::noAmbition;
}

Stampede::~Stampede() {}

void Stampede::BuildSentence() {

	_sentence += _identifier;
	_sentence += "And suddenly, a stampede of beasts could be seen in the distance!\n\n\n";
}

void Stampede::BuildGraph() {

	_plotGraph = Graph();

	if (_targetActor) {
		//GO TO ACTOR
		Node* goToNode = new Node();
		goToNode->SetNodeType(NodeType::goToActor);
		goToNode->SetActor(_targetActor->GetOwner());
		_plotGraph.AddNode(goToNode);

	}
	else {
		//GO TO LOCATION
		Node* goToNode = new Node();
		goToNode->SetNodeType(NodeType::goTo);
		goToNode->SetPosition(_targetLocation);
		_plotGraph.AddNode(goToNode);
	}

	// DESTROY WHEN FINISHED
	Node* destroySelfNode = new Node();
	destroySelfNode->SetNodeType(NodeType::destroySelf);
	_plotGraph.AddNode(destroySelfNode);
}

void Stampede::InitPlot() {

	_identifier = "Stampede:\n";
	BuildGraph();

	_heard = _plotGenerator->SpawnEntities(_num, _race, _spawnLocation);

	for (int i = 0; i < _heard.size(); i++) {

		_heard[i]->SetIdleGraph(&_plotGraph);
		_heard[i]->RethinkState();
	}

	BuildSentence();
}

BasePlot* Stampede::ConsiderReactions() {
	return nullptr;
}


// WAR
//*************************************************************************************

WarPlot::WarPlot(UOEntity* plotEntity) : BasePlot(plotEntity) {

	_plotEntity = plotEntity;
	_motivation = nullptr;
	_ambition = TypeOfAmbition::noAmbition;

	_targetEntity = _plotEntity->GetMostHated();
}

WarPlot::~WarPlot() {}

void WarPlot::BuildSentence() {

	_sentence += _identifier;
	_sentence += "Goblins and humans had been in conflict for over a hundred years, but things had gone too far.\nIt was time for war.\n\n\n";
}

void WarPlot::BuildGraph() {

	_plotGraph = Graph();

	Node* askForHelpNode = new Node();
	askForHelpNode->SetHighPriority(true);
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	//GET WEAPON
	Node* getNode = new Node();
	getNode->SetHighPriority(true);
	getNode->SetNodeType(NodeType::get);
	getNode->SetAffordableUse(OntologicFunctions::weapon);
	_plotGraph.AddNode(getNode);

	//ATTACK
	Node* attackNode = new Node();
	attackNode->SetHighPriority(true);
	attackNode->SetNodeType(NodeType::attack);
	attackNode->SetHighPriority(true);
	attackNode->SetEntity(_targetEntity);
	_plotGraph.AddNode(attackNode);
}

void WarPlot::InitPlot() {

	_identifier = "War:\n";
	BuildGraph();
	BuildSentence();
}

BasePlot*  WarPlot::ConsiderReactions() {
	return nullptr;
}

UOEntity* WarPlot::GetTargetEntity() {
	return _targetEntity;
}







