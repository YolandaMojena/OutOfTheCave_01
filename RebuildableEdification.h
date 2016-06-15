// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DestructibleMesh.h"
#include "RebuildableEdification.generated.h"

UCLASS()
class OUTOFTHECAVE_01_API ARebuildableEdification : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARebuildableEdification(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Edification)
		UDestructibleComponent* destructible;

	UDestructibleMesh* originalMesh;

	UDestructibleMesh* copy;

	bool RebuildEdification();
};
