// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"

#include "RopeNode.generated.h"

UCLASS()
class MYPROJECT_API ARopeNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARopeNode();

	UPROPERTY(EditAnywhere, Category = "Rope")
	USphereComponent * NodeSphere;

	UPROPERTY(EditAnywhere, Category = "Rope")
	UStaticMeshComponent * NodeMesh;
	
	UPROPERTY(EditAnywhere, Category = "Rope")
	UPhysicsConstraintComponent * NodeJoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
