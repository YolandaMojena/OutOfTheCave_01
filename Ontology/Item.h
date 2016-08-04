// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include <string>
#include <math.h>
#include "Item.generated.h"

using namespace std;

class UOOwnable;
class UOEntity;
class UOEdification;
class UItem;
class APlotGenerator;



UENUM(BlueprintType)
enum class ERace : uint8 {
	R_Human		UMETA(DisplayName = "Human"),
	R_Goblin	UMETA(DisplayName = "Goblin"),
	R_Mixt_HG	UMETA(DisplayName = "Mixt_HG"),

	R_Cow		UMETA(DisplayName = "Cow"),
	R_Goat		UMETA(DisplayName = "Goat"),
	R_Sheep		UMETA(DisplayName = "Sheep"),
	R_Boar		UMETA(DisplayName = "Boar"),
	R_Bear		UMETA(DisplayName = "Bear"),
	R_Wolf		UMETA(DisplayName = "Wolf"),
	R_Golem		UMETA(DisplayName = "Golem"),
	R_Beast		UMETA(DisplayName = "Beast"),
	R_Troll		UMETA(DisplayName = "Troll")
};

UENUM(BlueprintType)
enum class EJob : uint8 {
	J_Peasant		UMETA(DisplayName = "Peasant"),
	J_Farmer		UMETA(DisplayName = "Farmer"),
	J_Ironsmith		UMETA(DisplayName = "Ironsmith"),
	J_Soldier		UMETA(DisplayName = "Soldier"),
	J_Miner			UMETA(DisplayName = "Miner"),
	J_Shaman		UMETA(DisplayName = "Shaman"),

	J_Herbibore		UMETA(DisplayName = "Herbibore"),
	J_Predator		UMETA(DisplayName = "Predator")
};

UENUM(BlueprintType)
enum class ENotify : uint8 {
	N_Grabbed	UMETA(DisplayName = "Grabbed"),
	N_Damaged	UMETA(DisplayName = "Damaged")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OUTOFTHECAVE_01_API UItem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItem();
	~UItem();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString _name = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _density;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _mass; //CharacterMovement Component!?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _volume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _maxLength;
	/*float _elongation{
		get{
			return _maxLength / _volume;
		}
	}*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _toughness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FVector _grabPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FVector _grabDir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FVector _funcPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FVector _funcDir; //Think of an axe!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FVector _centerOfMass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _edgeSharpness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _edgeLength;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float inertiaMomentum;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _spiky;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _spikes;


	/*float _inertiaMomentum{
		get{
			return FVector.Distance(_grabPos, _funcPos);
		}
	}*/

	UOEntity* GetEntityComponent(AActor* actor);
	UOOwnable* GetOwnableComponent(AActor* actor);
	UOEdification* GetEdificationComponent(AActor* actor);
	APlotGenerator* GetPlotGenerator();

	float GetDensity();
	float GetMass();
	float GetVolume();

	float GetMaxLength();
	float GetElongation();

	float GetToughness();
	FVector GetGrabPos();
	FVector GetGrabDir();
	FVector GetFuncPos();
	FVector GetFuncDir(); //Think of an axe!
	FVector GetCenterOfMass();
	float GetAngularInertia();
	float GetAngularMomentumTransmission();
	float GetEdgeSharpness();
	float GetEdgeLength();
	float GetSpiky();
	float GetSpikes();
	FString GetItemName();
	int GetIntegrity();

	void SetItemName(FString name);

	void CastNotify(UItem* predicate, UOEntity* subject, ENotify notifyType);

protected:
	const float _NOTIFICATION_RADIUS = 1000.0f;

	enum _NotifyTag {
		damaged, destroyed, grabbed
	};

	// All entities will send reports to the plotGenerator situated in the game world
	APlotGenerator* _plotGenerator;

	//string GenerateNotifyID(UOOwnable* ownable, UOEntity* entity, _NotifyTag tag);
	//string GenerateNotifyID(UOEntity* pasiva, UOEntity* activa, _NotifyTag tag);
	FString GenerateNotifyID(UItem* predicate, UOEntity* subject, ENotify notifyType);

	float _integrity = 100;
};
