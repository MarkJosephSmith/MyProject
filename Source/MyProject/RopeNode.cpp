// Fill out your copyright notice in the Description page of Project Settings.

#include "RopeNode.h"


// Sets default values
ARopeNode::ARopeNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NodeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NodeRoot"));
	RootComponent = NodeSphere;
	NodeSphere->InitSphereRadius(5.0f);
	NodeSphere->SetCollisionProfileName(TEXT("Pawn"));

	NodeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NodeVisualMesh"));
	NodeMesh->SetupAttachment(RootComponent);

	NodeJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("NodeJoint"));
	NodeJoint->SetupAttachment(NodeMesh);
}

// Called when the game starts or when spawned
void ARopeNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARopeNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

