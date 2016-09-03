// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Item.h"
#include "OOwnable.h"
#include "NarrativeGeneration/PlotGenerator.h"
#include "OEntity.h"
#include "OEdification.h"


// Sets default values for this component's properties
UItem::UItem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

UItem::~UItem(){}


// Called when the game starts
void UItem::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<APlotGenerator> Itr(GetOwner()->GetWorld()); Itr; ++Itr)
		_plotGenerator = *Itr;
}


// Called every frame
void UItem::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}


/*string UItem::GenerateNotifyID(UOOwnable* ownable, UOEntity* entity, _NotifyTag tag) {
	return "" + ownable->GetUniqueID() + entity->GetUniqueID() + tag;
}
string UItem::GenerateNotifyID(UOEntity* pasiva, UOEntity* activa, _NotifyTag tag) {

	return "" + pasiva->GetUniqueID() + activa->GetUniqueID();
}*/
FString UItem::GenerateNotifyID(UItem* predicate, UOEntity* subject, ENotify notifyType) {

	return "" + FString::SanitizeFloat(predicate->GetUniqueID()) + FString::SanitizeFloat(subject->GetUniqueID()) + FString::SanitizeFloat((int)notifyType);
}

APlotGenerator* UItem::GetPlotGenerator() {
	return _plotGenerator;
}



/*float UItem::GetDensity() { return _mass / _volume; }
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
float UItem::GetAngularMomentumTransmission() { return (100.f / (FVector::Dist(_centerOfMass, _funcPos) * sin(acosf(FVector::DotProduct((_funcPos - _centerOfMass).GetSafeNormal(), _funcDir)))+1.0f)); }
//float UItem::GetAngularMomentumTransmission() { return FVector::Dist(_centerOfMass, _funcPos); }
float UItem::GetEdgeSharpness() { return _edgeSharpness; }
float UItem::GetEdgeLength() { return _edgeLength; }
float UItem::GetSpiky() { return _spiky; }
float UItem::GetSpikes() { return _spikes; }*/

int UItem::GetMass() { return _mass; }
int UItem::GetVolume() { return _volume; }
int UItem::GetToughness() { return _toughness; }
int UItem::GetEdgeSharpness() { return _edgeSharpness; }
int UItem::GetEdgeLength() { return _edgeLength; }
int UItem::GetSpiky() { return _spiky; }
int UItem::GetNbrSpikes() { return _nbrSpikes; }
int UItem::GetElongation() { return _elongation; }
//RESTRICTED
int UItem::R_GetMaxLength() { return _r_maxLength; }
FVector UItem::R_GetGrabPos() { return _r_grabPos; }
FVector UItem::R_GetGrabDir() { return _r_grabDir; }
FVector UItem::R_GetFuncPos() { return _r_funcPos; }
FVector UItem::R_GetFuncDir() { return _r_funcDir; }		//Think of an axe!
FVector UItem::R_GetFuncPlane() { return _r_funcPlane; }	//Think of an axe!
FVector UItem::R_GetCenterOfMass() { return _r_centerOfMass; }



int UItem::GetIntegrity() {
	return _integrity;
}

FString UItem::GetItemName()
{
	return _name;
}

void UItem::SetItemName(FString name)
{
	_name = name;
	GetOwner()->SetActorLabel(name);
}


void UItem::CastNotify(UItem* predicate, UOEntity* subject, ENotify notifyType) {
	float const _NOTIFY_RADIUS = 1000.0f;
	FVector start = GetOwner()->GetActorLocation() + FVector(0, 0, -_NOTIFY_RADIUS);
	FVector end = start + FVector(0, 0, _NOTIFY_RADIUS * 2);
	TArray<FHitResult> hitData;

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, GetOwner());
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	GetOwner()->GetWorld()->SweepMultiByChannel(
		hitData,
		start,
		end,
		FQuat(),
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(_NOTIFY_RADIUS),
		RV_TraceParams
		);

	FString notifyID = GenerateNotifyID(predicate, subject, notifyType);

	for (auto hr : hitData) {
		UOEntity* entity = hr.GetActor()->FindComponentByClass<UOEntity>();
		if (entity) {
			entity->ReceiveNotify(predicate, subject, notifyType, notifyID);
		}
	}
}