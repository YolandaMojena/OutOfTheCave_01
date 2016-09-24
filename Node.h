// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ontology/OntologicFunctions.h"
#include "Ontology/Item.h"
#include <vector>
#include <string>

using namespace std;

class BasePlot;
class Graph;
class UOEntity;
class UOOwnable;
class UOEdification;
class UOGrabbable;
class UOResidence;

UENUM(BlueprintType)
enum class NodeType : uint8 {
	goTo UMETA(DisplayName = "goTo"),
	goToActor UMETA(DisplayName = "goToActor"),
	wait UMETA(DisplayName = "wait"),
	waitUntilDaytime UMETA(DisplayName = "waitUntilDaytime"),
	interact UMETA(DisplayName = "interact"),
	enter UMETA(DisplayName = "enter"),
	attack UMETA(DisplayName = "attack"),
	get UMETA(DisplayName = "get"),
	grab UMETA(DisplayName = "grab"),
	steal UMETA(DisplayName = "steal"),
	destroy UMETA(DisplayName = "destroy"),
	//celebrate UMETA(DisplayName = "celebrate"),
	give UMETA(DisplayName = "give"),
	build UMETA(DisplayName = "build"),
	askForHelp UMETA(DisplayName = "askForHelp"),
	askTroll UMETA(DisplayName = "askTroll"),
	spawn UMETA(DisplayName = "spawn"),
	cultivate UMETA(DisplayName = "cultivate"),
	mine UMETA(DisplayName = "mine"),
	//gather UMETA(DisplayName = "gather"),
	help UMETA(DisplayName = "help"),
	numb UMETA(DisplayName = "numb"),
	stopFight UMETA(Displayname = "stopFight"),
	preyUpon UMETA(DisplayName = "preyUpon"),
	defend UMETA(DisplayName = "defend"),
	destroySelf UMETA(DisplayName = "destroySelf"),
	//ambush UMETA(DisplayName = "ambush"),
	flee UMETA(DisplayName = "flee"),
	releaseItem UMETA(DisplayName = "releaseItem")
};

class OUTOFTHECAVE_01_API Node
{

public:
	Node();
	~Node();

	class NBlackboard {
	public:

		UItem* item = nullptr;
		UOEntity* entity;
		AActor* actor;
		UOOwnable* ownable;
		OntologicFunctions::AffordableUse affordableUse;
		UOEdification* edification;
		UOResidence* residence;
		FVector position = FVector(-1, -1, -1);
		UOEntity* anotherEntity;
		float daytime = -1.0f;
		bool boolKey;
		float floatKey;
		bool branch;
		bool completedOk = true;
		bool isHighPriority = false;
		//UOEntity::State entityState;
	};

	void RewriteNode(vector<Node*> nodes);

	void SetItem(UItem* i);
	void SetEntity(UOEntity* e);
	void SetActor(AActor* a);
	void SetOwnable(UOOwnable* o);
	void SetAffordableUse(OntologicFunctions::AffordableUse au);
	void SetEdification(UOEdification* edf);
	void SetGrabbable(UOGrabbable* grb);
	void SetPosition(FVector v);
	void SetAnotherEntity(UOEntity* ae);
	void SetDaytime(float d);
	void SetBoolKey(bool b);
	void SetFloatKey(float f);
	void SetAsBranch();
	void SetCompletedOk();
	void SetHighPriority(bool isHP);
	void SetHighPriority();

	NBlackboard nBlackboard;
	vector<Node*> nextNodes;

	void SetNodeType(NodeType n);
	NodeType GetNodeType();

private:
	NodeType _nodeType;
	bool _completedOk = true;
};
