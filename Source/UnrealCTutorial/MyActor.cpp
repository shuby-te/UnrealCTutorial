// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

AMyActor::AMyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AMyActor::BeginPlay()
{

	Super::BeginPlay();
	
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Log, TEXT("Delta Time : %f"), DeltaTime);

}

