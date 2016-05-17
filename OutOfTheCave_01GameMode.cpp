// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OutOfTheCave_01GameMode.h"
#include "Engine.h"


AOutOfTheCave_01GameMode::AOutOfTheCave_01GameMode(const FObjectInitializer& ObjectInitializer)
{
	// set default pawn class to our Blueprinted character, the "_C" sufix indicates we are using the blueprint class, and not the blueprint asset
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("Pawn'/Game/Blueprints/BP_Troll.BP_Troll_C'"));
	if (PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}
}

void AOutOfTheCave_01GameMode::BeginPlay()
{
	Super::BeginPlay();

	StartMatch();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HELLO WORLD"));
	}
}




