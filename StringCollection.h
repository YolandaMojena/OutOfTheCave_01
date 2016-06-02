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

	string ATTACK_PLOT = "attack";
	string GATHER_PLOT = "gather";
};