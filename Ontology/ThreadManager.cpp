// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "ThreadManager.h"

bool ThreadManager::threadRunning = false;
TArray<UOEntity*> ThreadManager::customers = TArray<UOEntity*>();

UOEntity* ThreadManager::currentCustomer = nullptr;

ThreadManager::ThreadManager() {}

ThreadManager::~ThreadManager() {}

void ThreadManager::Tick()
{
	if (threadRunning) {
		if (FNearbyEntitiesFinder::IsThreadFinished()) {
			FNearbyEntitiesFinder::Shutdown();
			RequestFinished();
		}
	}
}

void ThreadManager::AddRequest(UOEntity* newCustomer)
{
	customers.Insert(newCustomer, 0);
	if (!threadRunning)
		FindNext();
}

void ThreadManager::FindNext()
{
	threadRunning = true;
	currentCustomer = customers.Pop();
	TArray<UOEntity*>* inputArray = &(currentCustomer->GetPlotGenerator()->allEntities);
	TArray<UOEntity*>* outputArray = currentCustomer->GetNearbyEntitiesPtr();
	outputArray->Empty();
	FNearbyEntitiesFinder::JoyInit(currentCustomer, inputArray, outputArray);
}

void ThreadManager::RequestFinished()
{
	threadRunning = false;
	currentCustomer->FinishedFindingNearbyEntities();
	if (customers.Num() > 0)
		FindNext();
}


void ThreadManager::SomeoneDied(UOEntity* corpse) {
	if(customers.Contains(corpse))
		customers.Remove(corpse);

	if (threadRunning) {
		if (currentCustomer == corpse) {
			FNearbyEntitiesFinder::Shutdown();
			threadRunning = false;
			if (customers.Num() > 0)
				FindNext();
		}
		else {
			FNearbyEntitiesFinder::RestartSearch();
		}
	}
}
