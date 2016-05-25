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

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	

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

	_chargingJump = false;
	
}

// Called when the game starts or when spawned
void ATroll::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I AM A BIG TROLL!"));	
		//ClientMessage("Test client message");
	}
}

// Called every frame
void ATroll::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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
	InputComponent->BindAction("Jump", IE_Released, this, &ATroll::Jump); // This jump structure will have to allow to charge the jump
	InputComponent->BindAction("PickUpMain", IE_Pressed, this, &ATroll::PickUpMain);
	InputComponent->BindAction("PickUpSecondary", IE_Pressed, this, &ATroll::PickUpSecondary);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ATroll::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ATroll::StopSprint);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); // These are built-in functions, but they can be substituted by custom ones
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

	FVector Start = GetActorLocation() - FVector(0, 0, GetActorLocation().Z + 256) + GetActorRotation().Vector() *256;
	FVector End = Start + FVector(0, 0, 256*2);

	//The trace data is stored here
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
		FCollisionShape::MakeSphere(256),
		RV_TraceParams
		);

	/*DrawDebugSphere(
		GetWorld(),
		Start,
		256,
		32,
		FColor(255, 0, 0),
		true,
		-1.f
		);

	DrawDebugSphere(
		GetWorld(),
		End,
		256,
		32,
		FColor(0, 255, 0),
		true,
		-1.f
		);*/

	if (HitData.bBlockingHit) {

		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, HitData.GetActor()->GetActorLabel());

		AttachToSocket(HitData.GetActor(), "mainSocket");
	}
}

void ATroll::PickUpSecondary() {

	FVector Start = GetActorLocation() - FVector(0, 0, GetActorLocation().Z + 256) + GetActorRotation().Vector() * 256;
	FVector End = Start + FVector(0, 0, 256 * 2);

	//The trace data is stored here
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
		FCollisionShape::MakeSphere(256),
		RV_TraceParams
		);

	if (HitData.bBlockingHit) {

		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, HitData.GetActor()->GetActorLabel());
		AttachToSocket(HitData.GetActor(), "secondarySocket");

	}
}

void ATroll::AttachToSocket(AActor* target, string socket) {


	if (GetMesh()->DoesSocketExist(socket.c_str()))
		target->AttachRootComponentTo(GetMesh(), socket.c_str(), EAttachLocation::SnapToTarget, true);
	else GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Blue, TEXT("Trying to Attach to non-existing socket"));

}


