// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Troll.h"
#include "Entity.h"
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
}

// Called when the game starts or when spawned
void ATroll::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Troll initialized"));	
	}
}

// Called every frame
void ATroll::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(isAttacking)
		if (!SkelMesh->AnimScriptInstance->Montage_IsPlaying(myMontage)) {
			isAttacking = false;	
		}
	
	for (TArray<const FAnimNotifyEvent*>::TIterator it = SkelMesh->AnimScriptInstance->AnimNotifies.CreateIterator(); it; ++it) {
		if ((*it)->NotifyName.ToString() == "AttackBegins") _canDamage = true;
		if ((*it)->NotifyName.ToString() == "AttackPerformed") _canDamage = false;
	}
}

// Called to bind functionality to input
void ATroll::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveRight", this, &ATroll::MoveRight);
	InputComponent->BindAxis("MoveForward", this, &ATroll::MoveForward);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::Jump); // This jump structure will have to allow to charge the jump
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

		/*DrawDebugSphere(
		GetWorld(),
		Start,
		_PICK_UP_RADIO,
		32,
		FColor(255, 0, 0),
		true,
		-1.f
		);*/

		// if hit, assign actor to main weapon and add overlap event
		if (HitData.bBlockingHit) {

			try {	
				// Will have to check if class is valid
				_mainWeapon = HitData.GetActor();
				GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, _mainWeapon->GetActorLabel());
				_mainWeapon->SetActorEnableCollision(false);


				TArray<USkeletalMeshComponent*> Components;
				_mainWeapon->GetComponents<USkeletalMeshComponent>(Components);
				for (int32 i = 0; i<Components.Num(); i++)
				{
					USkeletalMeshComponent* SkelMeshComponent = Components[i];
					SkelMeshComponent->SetSimulatePhysics(true);
					SkelMeshComponent->WakeAllRigidBodies();
				}
				AttachToSocket(_mainWeapon, "mainSocket");
				_mainWeapon->OnActorBeginOverlap.Add(HitFunc);
				_equipedMain = true;
			}
			catch(...){
				GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("Unhandled exception in Troll class"));
			}
		}
	}
	else {
		_equipedMain = false;
		_mainWeapon->OnActorBeginOverlap.Remove(HitFunc);
		_mainWeapon->DetachRootComponentFromParent(true);

		// HOW THE ACTOR IS LEFT ON THE FLOOR MUST BE SOLVED
		_mainWeapon->SetActorEnableCollision(true);
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

			try {
				// Will have to check if class is valid
				_secondaryWeapon = HitData.GetActor();
				GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, HitData.GetActor()->GetActorLabel());
				AttachToSocket(_secondaryWeapon, "secondarySocket");
				_secondaryWeapon->SetActorEnableCollision(false);
				_secondaryWeapon->OnActorBeginOverlap.Add(HitFunc);
				_equipedSecondary = true;
			}
			catch (...) {
				GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("Unhandled exception in Troll class"));
			}
		}
	}
	else {
		_equipedSecondary = false;
		_secondaryWeapon->OnActorBeginOverlap.Remove(HitFunc);
		_secondaryWeapon->SetActorEnableCollision(true);
		_secondaryWeapon->DetachRootComponentFromParent(true);
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

	if (isAttacking && _canDamage) {

		AEntity* hitEntity = dynamic_cast<AEntity*>(OtherActor);
		hitEntity->ReceiveDamage(_TROLL_DMG, this);
		//hitEntity->GetCharacterMovement()->Velocity += FVector(hitEntity->GetActorLocation().X - GetMesh()->GetSocketLocation("mainSocket").X, hitEntity->GetActorLocation().Y - GetMesh()->GetSocketLocation("mainSocket").Y, 0) * _attackDmg;
		//hitEntity->GetMesh()->AddForce()
	}
}



