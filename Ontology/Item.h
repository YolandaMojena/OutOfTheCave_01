// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include <string>
#include "Item.generated.h"

using namespace std;

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
	/*float _inertiaMomentum{
		get{
			return FVector.Distance(_grabPos, _funcPos);
		}
	}*/

};
