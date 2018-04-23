// Fill out your copyright notice in the Description page of Project Settings.

#include "Chain.h"


// Sets default values
AChain::AChain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChain::MakeAllLinksSeperated(int i_NumberOfLinks, float i_LinkRadius, FVector i_SeperationDistance, FVector i_ChainStart, AActor* i_pStartAttachment, AActor* i_pEndAttachment, FVector i_ChainEnd)
{
	//assert number of (links * linkradius +seperationdistance) < Magnitude of seperation distance
}

