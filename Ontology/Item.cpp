// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Item.h"
#include "OOwnable.h"
#include "OEntity.h"
#include "OEdification.h"


// Sets default values for this component's properties
UItem::UItem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

UOEntity* UItem::GetEntityComponent(AActor* actor) {

	UOEntity* foundComponent = actor->FindComponentByClass<UOEntity>();
	return foundComponent;
}
UOOwnable* UItem::GetOwnableComponent(AActor* actor) {

	UOOwnable* foundComponent = actor->FindComponentByClass<UOOwnable>();
	return foundComponent;
}

UOEdification * UItem::GetEdificationComponent(AActor * actor)
{
	UOEdification* foundComponent = actor->FindComponentByClass<UOEdification>();
	return foundComponent;
}



float UItem::GetDensity() { return _mass / _volume; }
float UItem::GetMass() { return _mass; }
float UItem::GetVolume() { return _volume; }

float UItem::GetMaxLength() { return _maxLength; }
float UItem::GetElongation() { return pow(_maxLength, 3) / _volume; }

float UItem::GetToughness() { return _toughness; }
FVector UItem::GetGrabPos() { return _grabPos; }
FVector UItem::GetGrabDir() { return _grabDir; }
FVector UItem::GetFuncPos() { return _funcPos; }
FVector UItem::GetFuncDir() { return _funcDir; } //Think of an axe!
FVector UItem::GetCenterOfMass() { return _centerOfMass; }
float UItem::GetAngularInertia() { return FVector::Dist(_grabPos, _centerOfMass); }
float UItem::GetAngularMomentumTransmission() { return (FVector::Dist(_centerOfMass, _funcPos) * sin(acosf(FVector::DotProduct((_funcPos - _centerOfMass).GetSafeNormal(), _funcDir)))); }
//float UItem::GetAngularMomentumTransmission() { return FVector::Dist(_centerOfMass, _funcPos); }
float UItem::GetEdgeSharpness() { return _edgeSharpness; }
float UItem::GetEdgeLength() { return _edgeLength; }
float UItem::GetSpiky() { return _spiky; }
float UItem::GetSpikes() { return _spikes; }

FString UItem::GetName()
{
	return _name;
}

void UItem::SetName(FString name)
{
	_name = name;
	GetOwner()->SetActorLabel(name);
}
