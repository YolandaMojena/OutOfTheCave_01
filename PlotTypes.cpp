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

		_sentence += "The brave " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName() 
			+ " began an attack against the despicable " + _targetEntity->GetRaceString()
			+ _targetEntity->GetName() + ", who had ";
		_sentence += _motivation->IsA<UOEntity>() ?
			" hurt his/her friend " + _motivation->GetName()
			: " damaged his/her " + _motivation->GetName();

		_sentence += ".\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::extermination) {

		_sentence += "The aggressive " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " began an attack against the unfortunate " + _targetEntity->GetRaceString() + " "
			+ _targetEntity->GetName() + ", in order to exterminate his/her race.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::notoriety) {

		_sentence += "The aggressive " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " began an attack against the highly notorious " + _targetEntity->GetRaceString() + " "
			+ _targetEntity->GetName() + ", in order to obtain his/her position in society.\n\n\n";
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
	getNode->SetAffordableUse(OntologicFunctions::AffordableUse::weapon);
	_plotGraph.AddNode(getNode);

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();
	ORelation* relation = _plotEntity->GetRelationWith(troll);
	if ( relation && relation->GetAppreciation() > 50) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::askTroll);
		askTrollForHelpNode->SetActor(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	//GO TO KILLER
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActor(_targetEntity->GetOwner());
	_plotGraph.AddNode(goToNode);

	//ATTACK
	Node* attackNode = new Node();
	attackNode->SetNodeType(NodeType::attack);
	attackNode->SetHighPriority(true);
	attackNode->SetEntity(_targetEntity);
	_plotGraph.AddNode(attackNode);
}

void AttackPlot::InitPlot() {

	_identifier = "Attack " + _targetEntity->GetName() + ":\n";
	BuildGraph();
	BuildSentence();
}

BasePlot* AttackPlot::ConsiderReactions() {

	BasePlot* reaction = nullptr;
	UOEntity* reactionEntity = nullptr;

	for (UOEntity* e : _involvedInPlot) {
		
		ORelation* relationWithMain = e->GetRelationWith(_plotEntity);
		ORelation* relationWithOther = e->GetRelationWith(_targetEntity);

		// SHOULD I VALUE RESPECT AND FEAR? 
		if (relationWithMain && relationWithOther && relationWithOther->GetAppreciation() > relationWithMain->GetAppreciation()) {
			
			DeleteFromInvolved(e);

			if (!reactionEntity) {
				reactionEntity = e;

				if (e->GetPersonality()->GetAggressiveness() > 50)
					reaction = new AmbushPlot(e, _plotEntity, _targetEntity, _targetEntity);
				else
					reaction = new DefendPlot(e, _plotEntity, _targetEntity, _targetEntity);
			}
			else if (reaction)
				reaction->AddInvolvedInPlot(e);
		}
	}
	return reaction;
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
	_ambition = TypeOfAmbition::noAmbition;
}

DestroyPlot::DestroyPlot(UOEntity* plotEntity, UOEntity* targetEntity, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_isExclusive = true;
	_ambition = ambition;
	_motivation = nullptr;
}

DestroyPlot::~DestroyPlot() {}

void DestroyPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {
		_sentence += "The brave " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " began to destroy the " + _targetEdification->GetName() +
			", which belongs to ";

		if (_targetEdification->GetOwners().size() > 0) {
			for (int i = 0; i < _targetEdification->GetOwners().size(); i++) {
				_sentence += _targetEdification->GetOwners()[i]->GetName();
				if (i < _targetEdification->GetOwners().size() - 2)
					_sentence += ", ";
				else _sentence += " and ";
			}
		}
		_sentence += ", since " + _targetEntity->GetName();
		_sentence += _motivation->IsA<UOEntity>() ?
			" had hurt his/her friend " + _motivation->GetName()
			: " had damaged his/her " + _motivation->GetName();

		_sentence += ".\n\n\n";
	}

	else if (_ambition == TypeOfAmbition::fear) {

		_sentence += "The coward " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
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

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActor(_targetEdification->GetOwner());
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

	if (_targetEdification) {
		_identifier = "Destroy " + _targetEdification->GetName() + ":\n";
		BuildGraph();
		BuildSentence();
	}
}

BasePlot* DestroyPlot::ConsiderReactions() {

	BasePlot* reaction = nullptr;
	UOEntity* reactionEntity = nullptr;

	for (UOEntity* e : _involvedInPlot) {

		ORelation* relationWithMain = e->GetRelationWith(_plotEntity);
		ORelation* relationWithOther = e->GetRelationWith(_targetEntity);

		// SHOULD I VALUE RESPECT AND FEAR? 
		if (relationWithMain && relationWithOther && relationWithOther->GetAppreciation() > relationWithMain->GetAppreciation()) {

			DeleteFromInvolved(e);

			if (!reactionEntity) {
				reactionEntity = e;

				if (e->GetPersonality()->GetAggressiveness() > 50)
					reaction = new AmbushPlot(e, _plotEntity, _targetEdification, _targetEntity);
				else
					reaction = new DefendPlot(e, _plotEntity, _targetEdification, _targetEntity);
			}
			else if (reaction)
				reaction->AddInvolvedInPlot(e);
		}
	}
	return reaction;
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
	_sentence += "The unfortunate " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName() + " had to rebuild his/her home, which had been destroyed by the merciless " + ((UOEntity*)_motivation)->GetRaceString() + " " + _motivation->GetName();
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

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActor(_targetEdification->GetOwner());
	_plotGraph.AddNode(goToNode);

	//BUILD
	Node* buildNode = new Node();
	buildNode->SetNodeType(NodeType::build);
	buildNode->SetEdification(_targetEdification);
	_plotGraph.AddNode(buildNode);
}

void BuildPlot::InitPlot() {

	_identifier = FString("Rebuild ") + _plotEntity->GetName() + FString("'s home:\n");
		
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
AmbushPlot::AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* target, UItem* motivation) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_target = target;
	_isExclusive = true;
	_motivation = motivation;
	_ambition = TypeOfAmbition::noAmbition;
}

AmbushPlot::AmbushPlot(UOEntity* plotEntity, UOEntity* targetEntity, UItem* target, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetEntity = targetEntity;
	_target = target;
	_isExclusive = true;
	_ambition = ambition;
	_motivation = nullptr;
}

AmbushPlot::~AmbushPlot() {}

void AmbushPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {

		_sentence += "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " committed an act of treason and organized an ambush against the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetName()
			+ ", who had planned to ";
		_sentence += _target->IsA<UOEntity>() ? " attack his/her friend " + _target->GetName() : " destroy his/her friend's " + _target->GetName();
		_sentence += ".\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::extermination) {

		_sentence += "The astute " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " assaulted the " + _targetEntity->GetRaceString() + "s! "
			+ "This time his/her victim was the unfortunate" + _targetEntity->GetName() +
			". He/she will go as far a neccesary to exterminate the race.\n\n\n";
	}
}

void AmbushPlot::BuildGraph() {

	_plotGraph = Graph();

	//GET WEAPON
	Node* getNode = new Node();
	getNode->SetNodeType(NodeType::get);
	getNode->SetAffordableUse(OntologicFunctions::weapon);
	_plotGraph.AddNode(getNode);

	// ASK FOR HELP
	Node* askForHelpNode = new Node();
	askForHelpNode->SetHighPriority(true);
	askForHelpNode->SetNodeType(NodeType::askForHelp);
	_plotGraph.AddNode(askForHelpNode);

	UOEntity* troll = UGameplayStatics::GetPlayerCharacter(_plotEntity->GetWorld(), 0)->FindComponentByClass<UOEntity>();
	ORelation* relation = _plotEntity->GetRelationWith(troll);
	if (relation && relation->GetAppreciation() > 50) {

		//ASK TROLL FOR HELP
		Node* askTrollForHelpNode = new Node();
		askTrollForHelpNode->SetNodeType(NodeType::askTroll);
		askTrollForHelpNode->SetActor(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	//GO TO ENEMY'S TARGET LOCATION
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::ambush);
	goToNode->SetActor(_targetEntity->GetCurrentTarget());
	_plotGraph.AddNode(goToNode);

	//ATTACK
	Node* attackNode = new Node();
	attackNode->SetNodeType(NodeType::attack);
	attackNode->SetHighPriority(true);
	attackNode->SetEntity(_targetEntity);
	_plotGraph.AddNode(attackNode);
}

void AmbushPlot::InitPlot() {

	_identifier = "Ambush against " + _targetEntity->GetName() + ":\n";
	BuildGraph();
	BuildSentence();
}

BasePlot* AmbushPlot::ConsiderReactions() {
	return nullptr;
}

UOEntity* AmbushPlot::GetTargetEntity() {
	return _targetEntity;
}

UItem* AmbushPlot::GetTarget() {
	return _target;
}


//STEAL PLOT
//**************************************************************************************

StealPlot::StealPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOOwnable* target, UItem* motivation) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_targetEntity = targetEntity;
	_motivation = motivation;
	_isExclusive = true;
	_ambition = TypeOfAmbition::noAmbition;
}

StealPlot::StealPlot(UOEntity* plotEntity, UOEntity* targetEntity, UOOwnable* target, TypeOfAmbition ambition) : BasePlot(plotEntity) {

	_targetOwnable = target;
	_targetEntity = targetEntity;
	_ambition = ambition;
	_isExclusive = true;
	_motivation = nullptr;
}

StealPlot::~StealPlot() {}

void StealPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {
		_sentence += "The vengeful " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " stole a " + _targetOwnable->GetName() + " from the " + _targetEntity->GetRaceString() +
			" " + _targetEntity->GetName() + ", since it was also stolen from him/her.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::possessions) {
		_sentence += "The materialistic " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " stole a precious " + _targetOwnable->GetName() + " from the " + _targetEntity->GetRaceString() +
			" " + _targetEntity->GetName() + ", since his/her ambition in life is to own lots of valuable possessions.\n\n\n";
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
	stealNode->SetEntity(_targetEntity);
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

	_identifier = "Steal " + _targetEntity->GetName() + "'s " + _targetOwnable->GetName() + "\n";
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
		_sentence += "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " was in need of a " + _targetOwnable->GetName()
			+ ". He/she trasversed the land to get it.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::possessions) {
		_sentence += "The materialistic " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " embarked on an adventure to find a precious " + _targetOwnable->GetName() +
			", since his/her ambition in life is to own lots of valuable possessions.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::appreciation) {
		_sentence += "The materialistic " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " embarked on an adventure to find a precious " + _targetOwnable->GetName() +
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
		askTrollForHelpNode->SetNodeType(NodeType::goToItem);
		askTrollForHelpNode->SetActor(troll->GetOwner());
		_plotGraph.AddNode(askTrollForHelpNode);
	}

	//GO TO OBJECT
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActor(_targetOwnable->GetOwner());
	_plotGraph.AddNode(goToNode);

	//GET OBJECT
	Node* grabNode = new Node();
	grabNode->SetNodeType(NodeType::grab);
	grabNode->SetOwnable(_targetOwnable);
	grabNode->SetActor(_targetOwnable->GetOwner());
	_plotGraph.AddNode(grabNode);
}

void GetPlot::InitPlot() {

	_identifier = "Get " + _targetOwnable->GetName() + " from world:\n";
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
		
		_sentence += "The kind " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " did his/her best to help the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetName()
			+ ", since he/she was really helpful to him/her in the past.\n\n\n";
	}

	else if (_ambition == TypeOfAmbition::appreciation) {

		_sentence += "The kind " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " did his/her best to help the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetName()
			+ ", since he/she would like to be more appreciated by him/her.\n\n\n";
	}

	else if (_ambition == TypeOfAmbition::notoriety) {

		_sentence += "The kind " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ " did his/her best to help the " + _targetEntity->GetRaceString() + " " + _targetEntity->GetName()
			+ ", since he/she is highly notorious in society. Maybe one day he/she'll get to be like him/her.\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::friendTroll) {

		_sentence += "The curious " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName()
			+ "wants to be the troll's friend! He/She's started following him around to learn from him and "
			+ "maybe, someday, do something useful for him.\n\n\n.";
	}
}

void HelpPlot::BuildGraph() {

	_plotGraph = Graph();

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActor(_targetEntity->GetOwner());
	_plotGraph.AddNode(goToNode);

	// HELP
	Node* helpNode = new Node();
	helpNode->SetNodeType(NodeType::help);
	helpNode->SetEntity(_targetEntity);
	if(_targetEntity->IsPlayer)
		helpNode->SetActor(_targetEntity->GetOwner());
	_plotGraph.AddNode(helpNode);

}

void HelpPlot::InitPlot() {

	_identifier = "The " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName() + " wants to help!:\n";

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

		_sentence += "The generours: " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName() + " is giving his precious " + _targetOwnable->GetName() + " to the " +
			_targetEntity->GetRaceString() + " " + _targetEntity->GetName() + ", in grattitude for the help received with his " + _motivation->GetName() + ".\n\n\n";
	}
	else if (_ambition == TypeOfAmbition::appreciation) {

		_sentence += "The generours: " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName() + " is giving a precious " + _targetOwnable->GetName() + " to the " +
			_targetEntity->GetRaceString() + " " + _targetEntity->GetName() + ", since he/she feels he/she would like to be more appreciated by others.\n\n\n";
	}
}

void GivePlot::BuildGraph() {

	_plotGraph = Graph();

	//INSERT GET IF NOT IN INVENTORY OR GRABBED//

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

void GivePlot::InitPlot() {

	_identifier = "Give " + _targetOwnable->GetName() + " to " + _targetEntity->GetName() + ":\n";

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


//DEFEND PLOT
//**************************************************************************************

DefendPlot::DefendPlot(UOEntity* plotEntity, UOEntity* against, UItem* target, UItem* motivation) : BasePlot(plotEntity) {

	_target = target;
	_against = against;
	_motivation = motivation;
	_ambition = TypeOfAmbition::noAmbition;
	_isExclusive = true;
}

DefendPlot::~DefendPlot() {}

void DefendPlot::BuildSentence() {

	_sentence += _identifier;

	if (_motivation) {

		_sentence += "And when everything seemed to be lost for the " + ((UOEntity*)_motivation)->GetRaceString() + " " + ((UOEntity*)_motivation)->GetName() +
			", his/her astute and preventive friend " + _plotEntity->GetRaceString() + " " + _plotEntity->GetName() + " came up with a defensive strategy to protect " ;
		_sentence += _target->IsA<UOEntity>() ? + "him/her " + _target->GetName() : + "his/her "  +_target->GetName() + " from the "
			+ _against->GetRaceString() + " " + _against->GetName();
			
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

	//GO TO TARGET
	Node* goToNode = new Node();
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActor(_target->GetOwner());
	_plotGraph.AddNode(goToNode);

	//GIVE
	Node* defendNode = new Node();
	defendNode->SetNodeType(NodeType::defend);
	defendNode->SetFloatKey(5);
	defendNode->SetActor(_against->GetOwner());
	_plotGraph.AddNode(defendNode);
}

void DefendPlot::InitPlot() {

	_identifier = "Defend for " + ((UOEntity*)_motivation)->GetRaceString() + " " + ((UOEntity*)_motivation)->GetName() + ":\n";

	BuildGraph();
	BuildSentence();
}

BasePlot* DefendPlot::ConsiderReactions() {
	return nullptr;
}



//STAMPEDE
//*************************************************************************************

Stampede::Stampede(ERace race, FVector spawnLocation, FVector targetLocation, float num, APlotGenerator* plotGenerator) {

	_race = race;
	_spawnLocation = spawnLocation;
	_targetLocation = targetLocation;
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
		goToNode->SetNodeType(NodeType::goToItem);
		goToNode->SetActor(_targetActor->GetOwner());
		_plotGraph.AddNode(goToNode);

	}
	else {
		//GO TO LOCATION
		Node* goToNode = new Node();
		goToNode->SetNodeType(NodeType::goTo);
		goToNode->SetPosition(_targetLocation * FVector(1, 1, 0));
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

	_heard = _plotGenerator->SpawnEntities(_num, _race);

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

	//GO TO MOST HATED
	Node* goToNode = new Node();
	goToNode->SetHighPriority(true);
	goToNode->SetNodeType(NodeType::goToItem);
	goToNode->SetActor(_targetEntity->GetOwner());
	_plotGraph.AddNode(goToNode);

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







