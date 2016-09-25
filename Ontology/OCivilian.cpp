// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OCivilian.h"

void UOCivilian::BeginPlay() {
	Super::BeginPlay();
}

void UOCivilian::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsInitialized && _isRecognized && !_notorietyCheck) {
		_notorietyCheck = true;
		if (rand() % 100 < 50) {
			ChangeNotoriety(+10);
			_plotGenerator->AddNotorious(this);
		}
	}
}



UTexture2D* UOCivilian::LoadTextureFromPath(const FString & Path)
{
	if (Path.IsEmpty()) return NULL;
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(Path)));
}

TypeOfAmbition UOCivilian::GetAmbition()
{
	return _ambition;
}

void UOCivilian::SetAmbition(TypeOfAmbition ambition)
{
	_ambition = ambition;
}


