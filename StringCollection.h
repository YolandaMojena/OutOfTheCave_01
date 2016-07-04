// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <string>

using namespace std;

class OUTOFTHECAVE_01_API StringCollection
{
public:

	StringCollection();
	~StringCollection();

	//ICON PATHS
	string ATTACK_ICON = "attack.attack'";
	string GOTO_ICON = "walk.walk'";
	string GATHER_ICON = "wood.wood'";
	string WAIT_ICON = "wait.wait'";

	// PLOT TYPES
	string ATTACK_PLOT = "attackPlot";
	string GATHER_PLOT = "gatherPlot";
	string ROBBERY_PLOT = "robberyPlot";
	string DESTROY_PLOT = "destroyPlot";
	string BUILD_PLOT = "buildPlot";
	string GIVE_PLOT = "givePlot";
	string HELP_PLOT = "helpPlot";

	// NODE TYPES
	string ATTACK_NODE = "attackNode";
	string GATHER_NODE = "gatherNode";
	string ROBBERY_NODE = "robberyNode";
	string DESTROY_NODE = "destroyNode";
	string GO_TO_NODE = "goToNode";
	string WAIT_NODE = "waitNode";
	string SEARCH_NODE = "searchNode";
	string GIVE_NODE = "giveNode";
};