// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include <string>
#include <vector>
#include <Relation.h>
#include <Troll.h>
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

	Relation::KindOfRelation RelationWithTroll();

	void AttackTroll(ATroll* player);

	void Attack(AEntity other);

	void ReceiveDamage(float damage);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LoadTextureFromPath", Keywords = "Load texture from path"), Category = Game)
		static UTexture2D* LoadTextureFromPath(const FString& Path);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		FString currentPath;

	vector<Relation> relations;

	
private:

	FString defaultPath = "Texture2D'/Game/Icons/";
	float velocity = 350.0f;
	float health = 1000.0f;
	float attackDmg = 50.0f;
};


