// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "PaperCharacter.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"				//"PlayerController.h"
#include "CableComponent.h"
#include "FuckingCable.h"
#include "CableActor.h"
#include "FuckingCableActor.h"
#include "MyProject/HookAttachement.h"
#include "MyProject/StupidPhysicsConstraintComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"


#include "CableComponent.h"
//#include "Runtime/Engine/Private/"
//#include "Runtime/Core/Public/"    ///"CableActor.h"

#include "MyProjectCharacter.generated.h"

class UTextRenderComponent;

/**
 * This class is the default character for MyProject, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AMyProjectCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hook)
	//UFuckingCable * MyCable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hook)
	UCableComponent * TestCable;//ACableActor * TestCable;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hook)
	AFuckingCableActor * FuckingCableActor;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hook)
	//class UChildActorComponent * FuckingCableChild;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hook)
	//TSubclassOf<class AFuckingCableActor> CableClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hook)
	bool bIsHooked;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UFUNCTION()
	void TestMouseLocationForCable();


	void ShootAndHook(FHitResult &ToHook);

	void ShootAndMiss(FVector CableEnd);

public:
	AMyProjectCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hook)
	float CableLength = 1000.0f;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
