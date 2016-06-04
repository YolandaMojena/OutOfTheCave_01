// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Item.h"
#include "OOwnable.h"
#include "OEntity.h"


// Sets default values for this component's properties
UItem::UItem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	name = TCHAR_TO_UTF8(*uname);
}


// Called when the game starts
void UItem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItem::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}


string UItem::GenerateNotifyID(UOOwnable* ownable, UOEntity* entity, _NotifyTag tag) {
	return "" + ownable->GetUniqueID() + entity->GetUniqueID() + tag;
}
string UItem::GenerateNotifyID(UOEntity* pasiva, UOEntity* activa, _NotifyTag tag) {

	return "" + pasiva->GetUniqueID() + activa->GetUniqueID();
}