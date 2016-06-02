// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include <string>
#include "Ontology/OEntity.h"
#include "Ontology/OCivilian.h"
#include "Ontology/OOwnable.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		UCameraComponent* FollowCamera;

	void ReceiveDamage(float attackDamage, AActor* damager);

	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Colliders)
		UStaticMeshComponent*  mainHandCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Colliders)
		UStaticMeshComponent*  secondaryHandCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Custom)
		bool isAttacking;

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
		void AttackMain();

	UFUNCTION()
		void AttackSecondary();

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
	UOEntity* GetEntityComponent(AActor* actor);
	UOOwnable* GetOwnableComponent(AActor* actor);

	float _health = 1000.0f;
	bool _canDamage;
	bool _equipedMain;
	bool _equipedSecondary;

	AActor* _mainWeapon;
	AActor* _secondaryWeapon;

	const float _PICK_UP_RADIO = 256.0f;
	const float _TROLL_DMG = 150.0f;

	USkeletalMeshComponent* SkelMesh;
	UAnimMontage* myMontage;
	TScriptDelegate<FWeakObjectPtr> HitFunc;

	bool _chargingJump;
	float _jumpMultiplier;
	const float _minJumpMultiplier = 0.75f;
	const float _growthJumpMultiplier = 0.5f;
	const float _maxJumpMultiplier = 1.5f;
	const float _averageJump = 1900.0f;

	const float _NORMAL_SPEED = 1200.0f;
	const float _SPRINT_SPEED = 2200.0f;
	const float _NORMAL_GROUNDFRICTION = 2.0f;
	const float _SPRINT_GROUNDFRICTION = 1.0f;

};
