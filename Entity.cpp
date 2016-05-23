// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Entity.h"


// Sets default values
AEntity::AEntity(const FObjectInitializer& ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_canBeHurt = true;
}

// Called when the game starts or when spawned
void AEntity::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentIcon("wait.wait'");	
}

// Called every frame
void AEntity::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!_canBeHurt) {
		_hurtCooldown+=DeltaTime;

		if (_hurtCooldown >= _HURT_COOLDOWN) {
			_hurtCooldown = 0;
			_canBeHurt = true;
		}
	}
}

// Called to bind functionality to input
void AEntity::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AEntity::SetCurrentIcon(FString icon)
{
	currentPath = _DEFAULT_PATH + icon;
}

UTexture2D * AEntity::LoadTextureFromPath(const FString & Path)
{
	if (Path.IsEmpty()) return NULL;
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(Path)));
}

vector<AEntity*> AEntity::RelatedWithOthers(Relation::KindOfRelation kind) {

	vector<AEntity*> others; 

	for (int i = 0; i < relations.size(); i++)
		if (relations[i].relation == kind) others.push_back(relations[i].otherEntity);

	return others;
}

vector<AThing*> AEntity::RelatedWithThings(Relation::KindOfRelation kind) {

	vector<AThing*> things;

	for (int i = 0; i < relations.size(); i++)
		if (relations[i].relation == kind) things.push_back(relations[i].otherThing);

	return things;
}

void AEntity::AttackTroll(ATroll* player)
{
	player->ReceiveDamage(_ATTACK_DMG, this);
}

void AEntity::ReceiveDamage(float attackDmg, AActor* punisher) {
	
	if (_canBeHurt && !_isDead) {
		_health -= attackDmg;
		_canBeHurt = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("" + GetActorLabel() + ": Auch! HP: " + FString::SanitizeFloat(_health)));

		if (_health <= 0)
			Die();
	}
}

void AEntity::GoToLocation(FVector worldPoint, float deltaTime) {

	UE_LOG(LogTemp, Warning, TEXT("Going"));

	FVector direction = FVector(worldPoint.X - GetActorLocation().X, worldPoint.Y - GetActorLocation().Y, 0);

	if (direction.Size() > 1) {

		FRotator lookAtDirection = FRotationMatrix::MakeFromX(direction).Rotator();
		SetActorRotation(lookAtDirection);

		direction.Normalize();
		SetActorLocation(FVector(GetActorLocation().X + direction.X * _velocity * deltaTime, GetActorLocation().Y + direction.Y * _velocity * deltaTime, GetActorLocation().Z));
	}
}

void AEntity::Die() {

	_isDead = true;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCastShadow(false);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("" + GetActorLabel() + ": Dead"));
	SetCurrentIcon("");

	//Set changes in relations with world

}

void AEntity::Attack(AEntity* enemy, float deltaTime) {

	UE_LOG(LogTemp, Warning, TEXT("Attacking"));

}

void AEntity::Gather(AThing* thing, float deltaTime) {

	UE_LOG(LogTemp, Warning, TEXT("Gathering"));

}

