// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 함수를 input과 묶기 위해서
//Setup : 설정
//Player : 플레이어의
//Input Component : 입력 컴포넌트
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super : APawn 
	//SetupPlayerInputComponent 호출
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

