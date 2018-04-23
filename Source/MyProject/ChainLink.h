// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/CapsuleComponent.h"
#include "Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "ChainLink.generated.h"

UCLASS()
class MYPROJECT_API AChainLink : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChainLink();

	UPROPERTY(EditAnywhere, Category = "Chain")
	float m_Radius;

	//The mesh we are using for the link
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chain")
	UStaticMeshComponent * m_pLinkMesh;

	//The physics joint component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chain")
	UPhysicsConstraintComponent * m_pPhysicsJoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;





	
	
};
