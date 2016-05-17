// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "StringCollection.h"
#include "PlotTypes.h"

//GO TO PLOT
//**************************************************************************************

GoToPlot::GoToPlot() {

	name = "GoToPlot";
	vector<Node*> graphNodes = { new BeginNode("Begin", strings.WAIT_ICON), new GoToNode("GoTo", strings.GOTO_ICON)};
	vector<Node::Arc>graphArcs = { Node::Arc("Begin", "GoTo", "")};

	BuildGraph(graphNodes, graphArcs);
}

GoToPlot::~GoToPlot() {}

void GoToPlot::BuildSentence() {

	FString entityName = plotEntity->GetActorLabel();
	string conversion(TCHAR_TO_UTF8(*entityName));

	sentence = "Entity " + conversion + " is going to " + targetLocation->locationName;

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, sentence.c_str());
}

void GoToPlot::GatherTargets(vector<AEntity*> candidates) {

	BuildSentence();

	plotEntity = candidates[rand() % candidates.size()];	

	targetLocation = new VenturLocation("TestLocation", FVector::ZeroVector);
}


//ATTACK PLOT
//**************************************************************************************

AttackPlot::AttackPlot() {

	name = "AttackPlot";
	vector<Node*> graphNodes = { new BeginNode("Begin",strings.WAIT_ICON), new GoToNode("GoTo", strings.GOTO_ICON), new AttackNode("Attack", strings.ATTACK_ICON), new GoToNode("Return", strings.GOTO_ICON) };
	vector<Node::Arc> graphArcs = { Node::Arc("Begin", "GoTo", ""), Node::Arc("GoTo", "Attack", "") };

	BuildGraph(graphNodes, graphArcs);
}

AttackPlot::~AttackPlot() {}

void AttackPlot::BuildSentence() {

	FString entityName = plotEntity->GetActorLabel();
	string conversion1(TCHAR_TO_UTF8(*entityName));

	FString enemyName = targetEntity->GetActorLabel();
	string conversion2(TCHAR_TO_UTF8(*enemyName));

	sentence = "Entity " + conversion1 + " is attacking " + conversion2;

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, sentence.c_str());
}

void AttackPlot::GatherTargets(vector<AEntity*> candidates) {

	plotEntity = candidates[rand() % candidates.size()];
	vector<AEntity*> foes = plotEntity->RelatedWithOthers(Relation::KindOfRelation::foe);
	targetEntity = foes[rand() % foes.size()];
	targetLocation = new VenturLocation("EnemyLocation", targetEntity->GetActorLocation());

	BuildSentence();
}


//GATHER PLOT
//**************************************************************************************

GatherPlot::GatherPlot() {

	name = "GatherPlot";

	vector<Node*> graphNodes = { new BeginNode("Begin", strings.WAIT_ICON), new GoToNode("GoTo", strings.GOTO_ICON), new GatherNode("Gather", strings.GATHER_ICON), new GoToNode("Return", strings.GOTO_ICON) };
	vector<Node::Arc> graphArcs = { Node::Arc("Begin", "GoTo", ""), Node::Arc("GoTo", "Gather", "") };

	BuildGraph(graphNodes, graphArcs);
}

GatherPlot::~GatherPlot() {}

void GatherPlot::BuildSentence() {

	FString entityName = plotEntity->GetActorLabel();
	string conversion1(TCHAR_TO_UTF8(*entityName));

	FString thingName = targetThing->GetActorLabel();
	string conversion2(TCHAR_TO_UTF8(*thingName));

	sentence = "Entity " + conversion1 + " is gathering " + conversion2;

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, sentence.c_str());
}

void GatherPlot::GatherTargets(vector<AEntity*> candidates) {

	plotEntity = candidates[rand() % candidates.size()];
	vector<AThing*> resources = plotEntity->RelatedWithThings(Relation::KindOfRelation::wants);
	targetThing = resources[rand() % resources.size()];
	targetLocation = new VenturLocation("ResourceLocation", targetThing->GetActorLocation());

	BuildSentence();
}




