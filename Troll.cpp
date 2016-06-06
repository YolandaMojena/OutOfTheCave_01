// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Troll.h"
#include "Engine.h"

// Sets default values
ATroll::ATroll(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character moves in the direction of input
	GetCharacterMovement()->bOrientRotationToMovement = true; 

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 650.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetWorldLocation(FVector(0, 0, 100.0f));

	// Create a follow camera
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOb_AW2(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	UStaticMesh* AssetSM_JoyControl = StaticMeshOb_AW2.Object;

	mainHandCollider = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("FistCollider"));
	mainHandCollider->SetStaticMesh(AssetSM_JoyControl);
	mainHandCollider->AttachParent = GetMesh();
	mainHandCollider->AttachSocketName = "mainSocket";
	mainHandCollider->bGenerateOverlapEvents = true;

	secondaryHandCollider = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SecondFistCollider"));
	secondaryHandCollider->SetStaticMesh(AssetSM_JoyControl);
	secondaryHandCollider->AttachParent = GetMesh();
	secondaryHandCollider->AttachSocketName = "secondarySocket";
	secondaryHandCollider->bGenerateOverlapEvents = true;

	HitFunc.BindUFunction(this, "OnOverlapBegin");
	mainHandCollider->OnComponentBeginOverlap.Add(HitFunc);
	mainHandCollider->SetVisibility(false, true);
	//secondaryHandCollider->OnComponentBeginOverlap.Add(HitFunc);
	secondaryHandCollider->SetVisibility(false, true);

	SkelMesh = FindComponentByClass<USkeletalMeshComponent>();
	ConstructorHelpers::FObjectFinder<UAnimMontage> anim_attack_montage(TEXT("AnimMontage'/Game/Animations/RunAttack.RunAttack'"));
	myMontage = anim_attack_montage.Object;

	_chargingJump = false;
	
}

// Called when the game starts or when spawned
void ATroll::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Troll initialized"));	
}

// Called every frame
void ATroll::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(isAttacking){
		if (!SkelMesh->AnimScriptInstance->Montage_IsPlaying(myMontage)) {
			isAttacking = false;	
		}
	}
	
	for (TArray<const FAnimNotifyEvent*>::TIterator it = SkelMesh->AnimScriptInstance->AnimNotifies.CreateIterator(); it; ++it) {
		if ((*it)->NotifyName.ToString() == "AttackBegins") _canDamage = true;
		if ((*it)->NotifyName.ToString() == "AttackPerformed") _canDamage = false;
	}

	if (_chargingJump && _jumpMultiplier < _maxJumpMultiplier) {
		_jumpMultiplier += DeltaTime * _growthJumpMultiplier;
	}
}

// Called to bind functionality to input
void ATroll::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveRight", this, &ATroll::MoveRight);
	InputComponent->BindAxis("MoveForward", this, &ATroll::MoveForward);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ATroll::ChargeJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ATroll::Jump);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ATroll::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ATroll::StopSprint);
	InputComponent->BindAction("PickUpMain", IE_Pressed, this, &ATroll::PickUpMain);
	InputComponent->BindAction("PickUpSecondary", IE_Pressed, this, &ATroll::PickUpSecondary);
	InputComponent->BindAction("AttackMain", IE_Pressed, this, &ATroll::AttackMain);
	InputComponent->BindAction("AttackSecondary", IE_Pressed, this, &ATroll::AttackSecondary);
	

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	/// To toggle camera rotation
	//InputComponent->BindAxis("TurnRate", this, &ATutorialCodeCharacter::TurnAtRate);
	//InputComponent->BindAxis("LookUpRate", this, &ATutorialCodeCharacter::LookUpAtRate);
}

void ATroll::ChargeJump() {
	_chargingJump = true;
	_jumpMultiplier = _minJumpMultiplier;
}

void ATroll::Jump() {
	_chargingJump = false;
	GetCharacterMovement()->JumpZVelocity = _averageJump * _jumpMultiplier;
	ACharacter::Jump();
}

void ATroll::StartSprint() {
	GetCharacterMovement()->MaxWalkSpeed = _SPRINT_SPEED;
	GetCharacterMovement()->GroundFriction = _SPRINT_GROUNDFRICTION;
}
void ATroll::StopSprint() {
	GetCharacterMovement()->MaxWalkSpeed = _NORMAL_SPEED;
	GetCharacterMovement()->GroundFriction = _NORMAL_GROUNDFRICTION;
}



void ATroll::MoveForward(float value) {

	if ((Controller != NULL) && (value != 0.0f)) {

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void ATroll::MoveRight(float value) {

	if ((Controller != NULL) && (value != 0.0f)) {

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void ATroll::PickUpMain() {

	if (!_equipedMain) {

		FVector Start = GetActorLocation() - FVector(0, 0, GetActorLocation().Z + _PICK_UP_RADIO) + GetActorRotation().Vector() * _PICK_UP_RADIO;
		FVector End = Start + FVector(0, 0, _PICK_UP_RADIO * 2);
		FHitResult HitData(ForceInit);

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		GetWorld()->SweepSingleByChannel(
			HitData,
			Start,
			End,
			FQuat(),
			ECollisionChannel::ECC_Visibility,
			FCollisionShape::MakeSphere(_PICK_UP_RADIO),
			RV_TraceParams
			);

		// if hit, assign actor to main weapon and add overlap event
		if (HitData.bBlockingHit) {

			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, HitData.GetActor()->GetActorLabel());

			_mainWeapon = HitData.GetActor();
			_mainRotation = _mainWeapon->GetActorRotation();

			if (GetEntityComponent(HitData.GetActor())) {

				UOEntity* hitEntity = GetEntityComponent(_mainWeapon);

				if (!hitEntity->GetIsDead()) {

					ACharacter* weaponChar = dynamic_cast<ACharacter*>(_mainWeapon);
					// BONE WILL PROBABLY DEPEND ON MESH
					weaponChar->GetMesh()->SetAllBodiesBelowSimulatePhysics(weaponChar->GetMesh()->GetBoneName(3), true);
				}
				// SOLVE PICKING UP DEAD ENTITIES (WITH SIMULATE PHYSICS ACTIVATED)
			}

			_mainWeapon->SetActorEnableCollision(false);
			AttachToSocket(_mainWeapon, "mainSocket");
			_mainWeapon->OnActorBeginOverlap.Add(HitFunc);
			_equipedMain = true;
		}
	}
	else {
		_equipedMain = false;
		_mainWeapon->OnActorBeginOverlap.Remove(HitFunc);
		_mainWeapon->DetachRootComponentFromParent(true);

		if (GetEntityComponent(_mainWeapon)) {

			UOEntity* hitEntity = GetEntityComponent(_mainWeapon);
			ACharacter* weaponChar = dynamic_cast<ACharacter*>(_mainWeapon);

			if (!hitEntity->GetIsDead()) {
				weaponChar->GetMesh()->SetAllBodiesSimulatePhysics(false);
			}
		}

		// HOW THE ACTOR IS LEFT ON THE FLOOR MUST BE SOLVED
		_mainWeapon->SetActorEnableCollision(true);
		_mainWeapon->SetActorRotation(_mainRotation);
		_mainWeapon = nullptr;
	}
}

void ATroll::PickUpSecondary() {

	if (!_equipedSecondary) {

		FVector Start = GetActorLocation() - FVector(0, 0, GetActorLocation().Z + _PICK_UP_RADIO) + GetActorRotation().Vector() * _PICK_UP_RADIO;
		FVector End = Start + FVector(0, 0, _PICK_UP_RADIO * 2);
		FHitResult HitData(ForceInit);

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		GetWorld()->SweepSingleByChannel(
			HitData,
			Start,
			End,
			FQuat(),
			ECollisionChannel::ECC_Visibility,
			FCollisionShape::MakeSphere(_PICK_UP_RADIO),
			RV_TraceParams
			);

		// if hit, assign actor to secondary weapon and add overlap event
		if (HitData.bBlockingHit) {
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, HitData.GetActor()->GetActorLabel());

			_secondaryWeapon = HitData.GetActor();
			_secondaryRotation = _secondaryWeapon->GetActorRotation();

			if (GetEntityComponent(HitData.GetActor())) {

				UOEntity* hitEntity = GetEntityComponent(_secondaryWeapon);

				if (!hitEntity->GetIsDead()) {

					ACharacter* weaponChar = dynamic_cast<ACharacter*>(_secondaryWeapon);
					weaponChar->GetMesh()->SetAllBodiesBelowSimulatePhysics(weaponChar->GetMesh()->GetBoneName(3), true);
				}
				// SOLVE PICKING UP DEAD ENTITIES (WITH SIMULATE PHYSICS ACTIVATED)

				_secondaryWeapon->SetActorEnableCollision(false);
				AttachToSocket(_secondaryWeapon, "secondarySocket");
				_secondaryWeapon->OnActorBeginOverlap.Add(HitFunc);
				_equipedSecondary = true;
			}
		}
	}
	else {
		_equipedSecondary = false;
		_secondaryWeapon->OnActorBeginOverlap.Remove(HitFunc);
		_secondaryWeapon->DetachRootComponentFromParent(true);

		if (GetEntityComponent(_secondaryWeapon)) {

			UOEntity* hitEntity = GetEntityComponent(_secondaryWeapon);
			ACharacter* weaponChar = dynamic_cast<ACharacter*>(_secondaryWeapon);

			if (!hitEntity->GetIsDead()) {
				weaponChar->GetMesh()->SetAllBodiesSimulatePhysics(false);
			}
		}

		// HOW THE ACTOR IS LEFT ON THE FLOOR MUST BE SOLVED
		_secondaryWeapon->SetActorEnableCollision(true);
		_secondaryWeapon->SetActorRotation(_secondaryRotation);
		_secondaryWeapon = nullptr;
	}
}

void ATroll::AttackMain() {

	if (!isAttacking) {
		isAttacking = true;
	}
}

void ATroll::AttackSecondary() {

}

void ATroll::ReceiveDamage(float attackDmg, AActor* punisher) {

	_health -= attackDmg;
}

void ATroll::AttachToSocket(AActor* target, string socket) {

	if (GetMesh()->DoesSocketExist(socket.c_str()))
		target->AttachRootComponentTo(GetMesh(), socket.c_str(), EAttachLocation::SnapToTarget, true);
	else GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("Trying to Attach to non-existing socket"));
}

void ATroll::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->GetActorLabel().Contains("_DM")){

		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("Hit a destrutible"));

		ADestructibleActor* targetDestructible = dynamic_cast<ADestructibleActor*>(OtherActor);
		targetDestructible->GetDestructibleComponent()->ApplyRadiusDamage(10, targetDestructible->GetDestructibleComponent()->GetCenterOfMass(), 32, 100, false);
	}
	else if (GetEntityComponent(OtherActor) && isAttacking && _canDamage) {

		UOEntity* hitEntity = GetEntityComponent(OtherActor);
		hitEntity->ReceiveDamage(_TROLL_DMG, GetEntityComponent(this));
		//FVector direction = FVector(hitEntity->GetActorLocation().X - GetMesh()->GetSocketLocation("mainSocket").X, hitEntity->GetActorLocation().Y - GetMesh()->GetSocketLocation("mainSocket").Y, 0);
		//FVector direction = FVector(this->GetActorLocation().X - GetMesh()->GetSocketLocation("mainSocket").X, this->GetActorLocation().Y - GetMesh()->GetSocketLocation("mainSocket").Y, 0);
		//hitEntity->GetOwner()->GetCharacterMovement()->Velocity += direction * _TROLL_DMG;
	}

	else if (GetOwnableComponent(OtherActor) && isAttacking && _canDamage) {

		UOOwnable* hitOwnable = GetOwnableComponent(OtherActor);
		hitOwnable->ReceiveDamage(_TROLL_DMG, this->FindComponentByClass<UOEntity>());
		//FVector direction = FVector(hitEntity->GetActorLocation().X - GetMesh()->GetSocketLocation("mainSocket").X, hitEntity->GetActorLocation().Y - GetMesh()->GetSocketLocation("mainSocket").Y, 0);
		//FVector direction = FVector(this->GetActorLocation().X - GetMesh()->GetSocketLocation("mainSocket").X, this->GetActorLocation().Y - GetMesh()->GetSocketLocation("mainSocket").Y, 0);
		//hitEntity->GetOwner()->GetCharacterMovement()->Velocity += direction * _TROLL_DMG;
	}
}

UOEntity* ATroll::GetEntityComponent(AActor* actor) {

	UOEntity* foundComponent = actor->FindComponentByClass<UOEntity>();
	return foundComponent;
}

UOOwnable* ATroll::GetOwnableComponent(AActor* actor) {

	UOOwnable* foundComponent = actor->FindComponentByClass<UOOwnable>();
	return foundComponent;
}
