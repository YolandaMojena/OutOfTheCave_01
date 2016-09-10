// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"

#include "Ontology/ThreadManager.h"

#include "FNearbyEntitiesFinder.h"

FNearbyEntitiesFinder* FNearbyEntitiesFinder::Runnable = NULL;

FNearbyEntitiesFinder::FNearbyEntitiesFinder(UOEntity* Entity, TArray<UOEntity*>* InputArray, TArray<UOEntity*>* OutputArray)
/*: ThePC(IN_PC)
,*/
	: TotalEntitiesToCheck(InputArray->Num())
	, StopTaskCounter(0)
	, EntitiesCheckedCount(0)
{
	TheEntity = Entity;
	EntitiesToCheck = InputArray;
	EntitiesFound = OutputArray;

	Thread = FRunnableThread::Create(this, TEXT("FNearbyEntitiesFinder"), 0, TPri_BelowNormal);
}

FNearbyEntitiesFinder::~FNearbyEntitiesFinder()
{
	delete Thread;
	Thread = NULL;
}

//Init
bool FNearbyEntitiesFinder::Init()
{
	//Init the Data
	//EntitiesFound.Empty();

	return true;
}

//Run
uint32 FNearbyEntitiesFinder::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	//While not told to stop this thread
	//		and not yet finished finding PrimeNumbers
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::FromHex("#11FF11"), TEXT("from a list of ") + FString::SanitizeFloat(EntitiesToCheck->Num()));

	while (StopTaskCounter.GetValue() == 0 && !IsFinished())
	{
		CheckNextEntity();
		EntitiesCheckedCount++;
	}

	//TheEntity->FinishedFindingNearbyEntities();
	//ThreadManager::RequestFinished();

	return 0;
}

//Stop
void FNearbyEntitiesFinder::Stop()
{
	StopTaskCounter.Increment();
}

FNearbyEntitiesFinder* FNearbyEntitiesFinder::JoyInit(UOEntity* Entity, TArray<UOEntity*>* InputArray, TArray<UOEntity*>* OutputArray)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FNearbyEntitiesFinder(Entity, InputArray, OutputArray);
	}
	return Runnable;
}

void FNearbyEntitiesFinder::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FNearbyEntitiesFinder::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool FNearbyEntitiesFinder::IsThreadFinished()
{
	if (Runnable)
		return Runnable->IsFinished();
	return true;
}

/*bool FNearbyEntitiesFinder::IsFinished() {
	return EntitiesCheckedCount >= TotalEntitiesToCheck;
}*/

void FNearbyEntitiesFinder::CheckNextEntity()
{
	const int RANGE = 1500;

	//UOEntity* currentEntity = (*EntitiesToCheck)[EntitiesCheckedCount];
	//UOEntity* currentEntity = (*EntitiesToCheck).operator[](EntitiesCheckedCount);
	//UOEntity* currentEntity = *EntitiesToCheck->FindByKey(EntitiesCheckedCount);
	//UOEntity* currentEntity = EntitiesToCheck[EntitiesCheckedCount];
	UOEntity* currentEntity = (*EntitiesToCheck)[EntitiesCheckedCount];
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::FromHex("#FF77FF"), currentEntity->GetItemName());

	if(currentEntity != TheEntity && FVector::Dist(currentEntity->GetOwner()->GetActorLocation(), TheEntity->GetOwner()->GetActorLocation()) < RANGE)
	{
		EntitiesFound->Add(currentEntity);
	}
}