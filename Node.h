// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <string>

using namespace std;

class BasePlot;
class Graph;
class UOEntity;
class UOOwnable;
class UOEdification;
class UOGrabbable;

UENUM(BlueprintType)
enum class NodeType : uint8 {
	branch UMETA(DisplayName = "branch"),
	goTo UMETA(DisplayName = "goTo"),
	goToItem UMETA(DisplayName = "goToItem"),
	wait UMETA(DisplayName = "wait"),
	interact UMETA(DisplayName = "interact"),
	enter UMETA(DisplayName = "enter"),
	attack UMETA(DisplayName = "attack"),
	get UMETA(DisplayName = "get"),
	grab UMETA(DisplayName = "grab"),
	steal UMETA(DisplayName = "steal"),
	destroy UMETA(DisplayName = "destroy"),
	celebrate UMETA(DisplayName = "celebrate"),
	give UMETA(DisplayName = "give"),
	build UMETA(DisplayName = "build"),
	askForHelp UMETA(DisplayName = "askForHelp"),
	askTroll UMETA(DisplayName = "askTroll"),
	spawn UMETA(DisplayName = "spawn"),
	cultivate UMETA(DisplayName = "cultivate"),
	gather UMETA(DisplayName = "gather")
};

class OUTOFTHECAVE_01_API Node
{

public:
	Node();
	~Node();

	class NBlackboard {
	public:

		UOEntity* entityA;
		UOEntity* entityB;
		AActor* actorA;
		UOOwnable* ownable;
		string arquetypeObject;
		UOEdification* edification;
		UOGrabbable* grabbable;
		FVector position;
		float daytime = -1.0f;
		bool branch;
		bool completedOk = true;
		//UOEntity::State entityState;
	};

	void RewriteNode(vector<Node*> nodes);
	void ExecuteTask();
	void NodeCompleted(bool completedOk);

	// Allows nullptr if a value is not required
	void PopulateBlackboard(UOEntity* entityA = nullptr, UOEntity* entityB = nullptr, UOOwnable* ownable = nullptr, UOEdification* edification = nullptr, UOGrabbable* grabbable = nullptr);
	void PopulateBlackboard(FVector position, UOEntity* entityA = nullptr, UOEntity* entityB = nullptr, UOOwnable* ownable = nullptr, UOEdification* edification = nullptr, UOGrabbable* grabbable = nullptr);

	void SetEntityA(UOEntity* e); //TBI
	void SetEntityB(UOEntity* e); //TBI
	void SetActorA(AActor* a); //TBI
	void SetOwnable(UOOwnable* o); //TBI
	void SetArquetypeObject(string s); //TBI
	void SetEdification(UOEdification* edf);
	void SetGrabbable(UOGrabbable* grb);
	void SetPosition(FVector v);
	void SetDaytime(float d);
	void SetAsBranch();
	void SetCompletedOk();
	//void SetEntityState(UOEntity::State s);

	NBlackboard nBlackboard;
	string name;
	vector<Node*> nextNodes;

	void SetNodeType(NodeType n);
	NodeType GetNodeType();

private:
	NodeType _nodeType;
	bool _completedOk = true;
};
