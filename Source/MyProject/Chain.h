// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChainLink.h"
#include "Chain.generated.h"

UCLASS()
class MYPROJECT_API AChain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Chain")
	void MakeAllLinksSeperated(int i_NumberOfLinks, float i_LinkRadius, FVector i_SeperationDistance, FVector i_ChainStart, AActor* i_pStartAttachment, AActor* i_pEndAttachment, FVector i_ChainEnd);

	/*
	void MakeAllLinksOverlap
	*/
	
	
};
