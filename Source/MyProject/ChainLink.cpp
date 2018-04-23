// Fill out your copyright notice in the Description page of Project Settings.

#include "ChainLink.h"


// Sets default values
AChainLink::AChainLink()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pLinkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChainLink"));
	RootComponent = m_pLinkMesh;

	m_pPhysicsJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ChainLinkJoint"));

	m_Radius = 50.0f;
}

// Called when the game starts or when spawned
void AChainLink::BeginPlay()
{
	Super::BeginPlay();

	if (m_pLinkMesh && m_pPhysicsJoint)
	{
		m_pPhysicsJoint->SetRelativeLocation( FVector::FVector(0, (m_Radius), 0));
		m_pLinkMesh->SetupAttachment(m_pPhysicsJoint);
	}

	
}

// Called every frame
void AChainLink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

