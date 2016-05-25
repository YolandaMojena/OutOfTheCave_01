// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include <string>
#include "Troll.generated.h"

using namespace std;

UCLASS()
class OUTOFTHECAVE_01_API ATroll : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATroll(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Camera boom positioning the camera behind the character 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

protected:

	UFUNCTION()
		void MoveForward(float value);

	UFUNCTION()
		void MoveRight(float value);

	UFUNCTION()
		void PickUpMain();

	UFUNCTION()
		void PickUpSecondary();

	UFUNCTION()
		void ChargeJump();

	UFUNCTION()
		void Jump();

	UFUNCTION()
		void StartSprint();

	UFUNCTION()
		void StopSprint();

private:

	void AttachToSocket(AActor* target, string socket);

	bool _chargingJump;
	float _jumpMultiplier;
	const float _minJumpMultiplier = 0.75f;
	const float _growthJumpMultiplier = 0.5f;
	const float _maxJumpMultiplier = 1.5f;
	const float _averageJump = 1600.0f;

	const float _NORMAL_SPEED = 1200.0f;
	const float _SPRINT_SPEED = 2400.0f;
	const float _NORMAL_GROUNDFRICTION = 2.0f;
	const float _SPRINT_GROUNDFRICTION = 1.0f;

};
