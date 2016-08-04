// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OwnableSpawner.h"


// Sets default values
AOwnableSpawner::AOwnableSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //!!!!


	static ConstructorHelpers::FObjectFinder<UBlueprint> HammerBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Hammer.Hammer'"));
	if (HammerBlueprint.Object) {
		BP_Hammer = (UClass*)HammerBlueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> HoeBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Hoe.Hoe'"));
	if (HoeBlueprint.Object) {
		BP_Hoe = (UClass*)HoeBlueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> PickaxeBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Pickaxe.Pickaxe'"));
	if (PickaxeBlueprint.Object) {
		BP_Pickaxe = (UClass*)PickaxeBlueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> RakeBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Rake.Rake'"));
	if (RakeBlueprint.Object) {
		BP_Rake = (UClass*)RakeBlueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> SwordBlueprint(TEXT("Blueprint'/Game/Blueprints/Ownables/Sword2.Sword2'"));
	if (SwordBlueprint.Object) {
		BP_Sword = (UClass*)SwordBlueprint.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> LeaflessTreeBlueprint(TEXT("Blueprint'/Game/Blueprints/LeaflessTree.LeaflessTree'"));
	if (LeaflessTreeBlueprint.Object) {
		BP_LeaflessTree = (UClass*)LeaflessTreeBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AOwnableSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOwnableSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


AActor* AOwnableSpawner::SpawnOwnable(UOOwnable* o, UItem* spawner) {
	AActor* spawned;
	FActorSpawnParameters SpawnParams;
	if (o->GetItemName() == "hammer") {
		spawned = spawner->GetWorld()->SpawnActor<AActor>(BP_Hammer, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "hoe") {
		spawned = spawner->GetWorld()->SpawnActor<AActor>(BP_Hoe, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "pickaxe") {
		spawned = spawner->GetWorld()->SpawnActor<AActor>(BP_Pickaxe, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "rake") {
		spawned = spawner->GetWorld()->SpawnActor<AActor>(BP_Rake, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else if (o->GetItemName() == "sword") {
		spawned = spawner->GetWorld()->SpawnActor<AActor>(BP_Sword, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	else { //else if (o->GetItemName() == "leaflesstree") {
		spawned = spawner->GetWorld()->SpawnActor<AActor>(BP_LeaflessTree, spawner->GetOwner()->GetActorLocation() + RandomDisplacementVector(500), spawner->GetOwner()->GetActorRotation(), SpawnParams);
	}
	return spawned;
}


FVector AOwnableSpawner::RandomDisplacementVector(int radius) {
	return FVector(rand() % (2 * radius) - radius, rand() % (2 * radius) - radius, 0);
}