// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "RebuildableEdification.h"


// Sets default values
ARebuildableEdification::ARebuildableEdification(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	destructible = ObjectInitializer.CreateDefaultSubobject<UDestructibleComponent>(this, TEXT("DM"));
}

// Called when the game starts or when spawned
void ARebuildableEdification::BeginPlay()
{
	Super::BeginPlay();

	originalMesh->SetFlags(RF_Public);

	destructible->SetDestructibleMesh(originalMesh);

	_copy = DuplicateObject(originalMesh, nullptr);
	
}

// Called every frame
void ARebuildableEdification::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


}

bool ARebuildableEdification::RebuildEdification() {

	if(destructible && _copy)
		destructible->SetDestructibleMesh(_copy);
	return true;
}

