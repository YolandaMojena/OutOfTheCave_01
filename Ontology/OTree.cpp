// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OTree.h"


UOTree::UOTree() {

	static ConstructorHelpers::FObjectFinder<UBlueprint> TreeBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Tree.BP_Tree'"));
	if (TreeBlueprint.Object) {
		BP_Tree = (UClass*)TreeBlueprint.Object->GeneratedClass;
	}
}

UOTree::~UOTree() {}

void UOTree::SpawnLeaflessTree()
{
	FActorSpawnParameters SpawnParams;
	AActor* tree = GetOwner()->GetWorld()->SpawnActor<AActor>(BP_Tree, GetOwner()->GetActorLocation() , GetOwner()->GetActorRotation(), SpawnParams);
	GetOwner()->Destroy();

}
