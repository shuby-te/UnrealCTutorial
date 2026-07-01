// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"


AArrow::AArrow()
{
 	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("/Game/ParagonSparrow/FX/Meshes/Heroes/Sparrow/Abilities/SM_Sparrow_Arrow.SM_Sparrow_Arrow"));

	if (SM.Succeeded())
	{
		Mesh->SetStaticMesh(SM.Object);
	}

	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeLocationAndRotation(FVector(100.0, 0.0, 0.0), FRotator(90.0, 0.0, 0.0));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
	CollisionBox->SetupAttachment(Mesh);
	CollisionBox->SetRelativeLocation(FVector(0.0, 0.0, -55.0));
	CollisionBox->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));


}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

