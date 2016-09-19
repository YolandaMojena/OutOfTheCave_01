// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ontology/OEntity.h"
#include "ThreadingBase.h"

class ThreadManager;

/**
 * OUTOFTHECAVE_01_API 
 */
class FNearbyEntitiesFinder : public FRunnable
{
	// Singleton instance
	static FNearbyEntitiesFinder* Runnable;

	// Thread to run the worker FRunnable on
	FRunnableThread* Thread;

	// The Data Ptrss
	TArray<UOEntity*>* EntitiesToCheck;
	TArray<UOEntity*>* EntitiesFound;
	UOEntity* TheEntity;

	// The PC
	//AVictoryGamePlayerController* ThePC;

	// Stop this thread? Uses Thread Safe Counter
	FThreadSafeCounter StopTaskCounter;

	void CheckNextEntity();

private:
	int32 EntitiesCheckedCount;
	int32 TotalEntitiesToCheck;

public:
	//FNearbyEntitiesFinder();
	//~FNearbyEntitiesFinder();

	bool IsFinished() const {
		return EntitiesCheckedCount >= EntitiesToCheck->Num();
		//return EntitiesCheckedCount >= TotalEntitiesToCheck;
	}
	//bool IsFinished();

	//~~~~ Thread Core Functions ~~~~

	// Constructor / Destructor
	FNearbyEntitiesFinder(UOEntity* Entity, TArray<UOEntity*>* InputArray, TArray<UOEntity*>* OutputArray);
	virtual ~FNearbyEntitiesFinder();


	// Begin FRunnable interface
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End Frunnable interface

	// Makes sure this thread has stopped properly
	void EnsureCompletion();


	//~~~~ Starting and Stopping Thread ~~~~
	static FNearbyEntitiesFinder* JoyInit(UOEntity* Entity, TArray<UOEntity*>* InputArray, TArray<UOEntity*>* OutputArray);

	// Shuts down the thread. Static so it can easily be called from outside the thread context
	static void Shutdown();

	static void RestartSearch();

	static bool IsThreadFinished();

};
