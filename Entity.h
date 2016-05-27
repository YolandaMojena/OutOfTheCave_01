// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include <string>
#include <vector>
#include <Relation.h>
#include <Troll.h>
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Entity.generated.h"

using namespace std;

UCLASS()
class OUTOFTHECAVE_01_API AEntity : public ACharacter
{
	GENERATED_BODY()

public:

	AEntity(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetCurrentIcon(FString icon);
	void GoToLocation(FVector worldPoint, float deltaTime);
	void Attack(AEntity* enemy, float deltaTime);
	void Gather(AThing* thing, float deltaTime);

	vector<AEntity*> RelatedWithOthers(Relation::KindOfRelation kind);
	vector<AThing*> RelatedWithThings(Relation::KindOfRelation kind);

	void AttackTroll(ATroll* player);
	void ReceiveDamage(float damage, AActor* punisher);
	void BePushedAround();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LoadTextureFromPath", Keywords = "Load texture from path"), Category = Game)
		static UTexture2D* LoadTextureFromPath(const FString& Path);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		FString currentPath;

	UPROPERTY(EditAnywhere, Category = Behaviour)
		class UBehaviorTree* entityBehavior;

	bool GetIsDead();

	vector<Relation> relations;
	Relation::KindOfRelation relationWithPlayer;
	
private:

	void Die();
	
	float _velocity = 350.0f;
	float _health = 150.0f;
	bool _canBeHurt;
	float _hurtCooldown;
	float _noCollisionCooldown;
	bool _isDead = false;

	const float _HURT_COOLDOWN = 0.75f;
	const FString _DEFAULT_PATH = "Texture2D'/Game/Icons/";
	const float _ATTACK_DMG = 50.0f;
	const float _NO_COLLISION_COOLDOWN = 1.0f;
};


