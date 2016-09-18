// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ontology/OEntity.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "FNearbyEntitiesFinder.h"

/**
 * 
 */
//using namespace std;

class OUTOFTHECAVE_01_API ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	static void AddRequest(UOEntity* customer);
	static void RequestFinished();
	static void Tick();

	static void SomeoneDied(UOEntity* corpse);

private:

	static void FindNext();

	static TArray<UOEntity*> customers;
	static bool threadRunning;
	static UOEntity* currentCustomer;
};
