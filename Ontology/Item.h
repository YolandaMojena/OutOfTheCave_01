// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include <string>
#include "Item.generated.h"

using namespace std;

class UOOwnable;
class UOEntity;
class UOEdification;



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
	R_Beast		UMETA(DisplayName = "Beast")
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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OUTOFTHECAVE_01_API UItem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItem();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString uname;

	string name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _density;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _mass; //CharacterMovement Component!?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _volume;
	/*float _volume{
		get{
			return _mass / _density;
		}
		set(value){
			_mass = _density * value
		}
	}*/
	/*float _mass{
	get{
	return _density * _volume;
	}
	}*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _maxLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _elongation;
	/*float _elongation{
		get{
			return _maxLength / volume;
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
		float _edgeSharpness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _edgeLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _inertiaMomentum;

	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float _integrity;

	float _canBeDamaged;
	const float _HURT_COOLDOWN = 0.75f;
	float _hurtCooldown;
	/*float _inertiaMomentum{
		get{
			return FVector.Distance(_grabPos, _funcPos);
		}
	}*/

	UOEntity* GetEntityComponent(AActor* actor);
	UOOwnable* GetOwnableComponent(AActor* actor);
	UOEdification* GetEdificationComponent(AActor* actor);

protected:

	const float _NOTIFICATION_RADIUS = 1000.0f;

	enum _NotifyTag {
		damaged, destroyed, grabbed
	};

	string GenerateNotifyID(UOOwnable* ownable, UOEntity* entity, _NotifyTag tag);
	string GenerateNotifyID(UOEntity* pasiva, UOEntity* activa, _NotifyTag tag);
};
