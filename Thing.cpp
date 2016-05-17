// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Thing.h"


// Sets default values
AThing::AThing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AThing::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AThing::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

