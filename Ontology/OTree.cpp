// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OTree.h"


UOTree::UOTree() {

	static ConstructorHelpers::FObjectFinder<UBlueprint> TreeBlueprint(TEXT("Blueprint'/Game/Blueprints/LeaflessTree.LeaflessTree'"));
	if (TreeBlueprint.Object) {
		BP_Tree = (UClass*)TreeBlueprint.Object->GeneratedClass;
	}
}

UOTree::~UOTree() {}

void UOTree::SpawnLeaflessTree()
{
	FActorSpawnParameters SpawnParams;
	AActor* tree = GetOwner()->GetWorld()->SpawnActor<AActor>(BP_Tree, GetOwner()->GetActorLocation() , GetOwner()->GetActorRotation(), SpawnParams);
	tree->SetActorScale3D(GetOwner()->GetActorScale3D());
	tree->SetActorLocation(GetOwner()->GetActorLocation() + FVector(0, 0, 253) * tree->GetActorScale3D().Z);
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("Tree becoming leafless"));
	GetOwner()->Destroy();

}
