// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	//FuckingCableChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("SpawnedCable"));
	//FuckingCableChild->SetupAttachment(RootComponent);
	//FuckingCableChild->SetChildActorClass(CableClass);

	//FuckingCableActor = CreateDefaultSubobject<AFuckingCableActor>(TEXT("SpawnedCable"));
	//FuckingCableActor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);    //SetupAttachment(RootComponent);

	//TestCable = CreateDefaultSubobject<UCableComponent>(TEXT("SpawnedCable"));
	//TestCable->SetupAttachment(GetCapsuleComponent());

}

void AMyProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	/*Show mouse cursor for grapple targetting*/
	APlayerController*  MyStoredController = Cast<APlayerController>(GetController());
	MyStoredController->bShowMouseCursor = true;
	MyStoredController->bEnableClickEvents = true;
	MyStoredController->bEnableMouseOverEvents = true;

	if (TestCable)
	{
		TestCable->bAttachStart = true;
		TestCable->bAttachEnd = true;
		TestCable->EndLocation = FVector::ZeroVector;
		TestCable->CableLength = 100;
		TestCable->NumSegments = 20;
		TestCable->SolverIterations = 16;
		TestCable->SubstepTime = 0.005f;
		TestCable->bEnableStiffness = true;
		TestCable->bEnableCollision = true;
		TestCable->CollisionFriction = 1.0f;
		TestCable->CableForce = FVector::ZeroVector;
		TestCable->CableGravityScale = 1.2;
		TestCable->CableWidth = 2.0f;
		TestCable->NumSides = 10;
	}

}

//////////////////////////////////////////////////////////////////////////
// Animation

void AMyProjectCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AMyProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("FireCable", IE_Released, this, &AMyProjectCharacter::TestMouseLocationForCable);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProjectCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyProjectCharacter::TouchStopped);
}

void AMyProjectCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AMyProjectCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AMyProjectCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AMyProjectCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void AMyProjectCharacter::TestMouseLocationForCable()
{





	FHitResult TheHit;
	FCollisionQueryParams CollisionParams;

	//Eventually want to make this get the hand location I think but need an animation for that.
	FVector CurrentLocation = this->GetActorLocation();
	FVector MouseLocation;
	FVector MouseDirection;
	APlayerController*  MyStoredController = Cast<APlayerController>(GetController());
	UWorld * TheWorld = GetWorld();

	if (MyStoredController && TheWorld)     //&&      (!MyCable))
	{

		if (MyStoredController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
		{
			/*Show debug trace lines*/
			const FName TraceTag("ShowTraceLine");
			TheWorld->DebugDrawTraceTag = TraceTag;
			CollisionParams.TraceTag = TraceTag;
			/*End show debug trace lines*/

			//correct the Y value for the line trace.
			MouseLocation.Y = CurrentLocation.Y;

			FVector TrajectoryToMouse = MouseLocation - CurrentLocation;
			TrajectoryToMouse.Normalize();

			//perform trace
			if (TheWorld->LineTraceSingleByChannel(TheHit, CurrentLocation, CurrentLocation+(TrajectoryToMouse * CableLength), ECC_GameTraceChannel1, CollisionParams))
			{
				UE_LOG(LogTemp, Log, TEXT("Hit Actor Location %s"), *TheHit.GetActor()->GetActorLocation().ToString());
				ShootAndHook(TheHit);

			}
			else
			{
				ShootAndMiss(MouseLocation);
			}





		}
		else
		{
			//shouldn't be firing
			check(false);
		}
	}
}  

void AMyProjectCharacter::ShootAndHook(FHitResult &ToHook)
{
	AActor * HitActor = ToHook.GetActor();
	float CLength = ((this->GetActorLocation()) - (ToHook.ImpactPoint)).Size();                  //(HitActor->GetActorLocation())).Size();
	

	
	//NewObject < AFuckingCableActor > (this, ACableActor::StaticClass());//TestCable = GetWorld()->SpawnActor<ACableActor>(ACableActor::StaticClass(),this->GetTransform(), CableSpawn) ; //NewObject<ACableActor>(this, ACableActor::StaticClass());
	UWorld * TheWorld = GetWorld();

	if (TheWorld)
	{
		FActorSpawnParameters CableSpawnParams;
		CableSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FuckingCableActor = TheWorld->SpawnActor<AFuckingCableActor>(AFuckingCableActor::StaticClass(), CableSpawnParams);
		FuckingCableActor->MyCable->CableLength = CLength;
		
		FuckingCableActor->MyCable->bAttachEnd = false;
		FuckingCableActor->MyCable->EndLocation = ToHook.ImpactPoint;
		
		FuckingCableActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName( "HookSocket"));

		FuckingCableActor->MyCable->RegisterComponent();
		
	}
	
	
	FName HookHit("HookLocation");
	UHookAttachement * HookComp = NewObject<UHookAttachement>(HitActor, UHookAttachement::StaticClass()); //<UHookAttachement>(UHookAttachement::StaticClass(), HitActor, HookHit);//UPhysicsConstraintComponent* HookComp = NewObject<UPhysicsConstraintComponent>(HitActor, UPhysicsConstraintComponent::StaticClass());  //UStupidPhysicsConstraintComponent* HookComp = NewObject<UStupidPhysicsConstraintComponent>(HitActor, UStupidPhysicsConstraintComponent::StaticClass()); //(UHookAttachement::StaticClass(), HitActor, HookHit);
																										 
	if (HookComp)
	{
		HookComp->RegisterComponent();
		HookComp->AttachToComponent(HitActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);  //SetupAttachment(HitActor->GetRootComponent());    //AttachTo(HitActor->GetRootComponent());
		HookComp->SetWorldLocation(ToHook.ImpactPoint);

		HookComp->SetWorldRotation(this->GetActorRotation());

		FString  toConvert = HookComp->GetName();
		FName HookHitName = FName(*toConvert);

		FVector HitPoint = ToHook.ImpactPoint;
		HitPoint.Y = 0;

		UE_LOG(LogTemp, Log, TEXT("Impact : %s"), *HitPoint.ToString());
	}

	//HookComp->ConstraintActor1 = HitActor;
	//HookComp->ConstraintActor2 = this;

	
	//FuckingCableActor->MyCable->SetAttachEndTo(HitActor, HookComp->GetFName());
	FName newName = HookComp->GetFName();
	FComponentReference newRef;
	newRef.ComponentProperty = newName;
	newRef.OtherActor = HitActor;
	newRef.OverrideComponent = HookComp;
	FuckingCableActor->MyCable->AttachEndTo = newRef;
	FuckingCableActor->MyCable->bAttachEnd = true;
	FuckingCableActor->MyCable->SetAttachEndTo(HitActor, newName);


	//FuckingCableActor->MyCable->EndLocation = FVector::ZeroVector;
	
}

void AMyProjectCharacter::ShootAndMiss(FVector CableEnd)
{
	bIsHooked = false;
}
