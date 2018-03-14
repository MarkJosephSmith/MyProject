// Fill out your copyright notice in the Description page of Project Settings.

#include "FuckingCableActor.h"


// Sets default values
AFuckingCableActor::AFuckingCableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyCable = CreateDefaultSubobject<UFuckingCable>(TEXT("CableComponent0"));
	RootComponent = MyCable;

}

// Called when the game starts or when spawned
void AFuckingCableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFuckingCableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

