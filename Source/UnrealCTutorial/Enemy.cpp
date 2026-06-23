// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"

AEnemy::AEnemy()
{
 	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -90.0), FRotator(0.0, -90.0, 0.0));

	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Enemy.ABP_Enemy_C'"));
	if (AI.Succeeded())
	{
		GetMesh()->SetAnimClass(AI.Class);
	}

	AIControllerClass = AEnemyAIController::StaticClass();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Damaged : %f"), Damage);
	return 0.0f;
}

