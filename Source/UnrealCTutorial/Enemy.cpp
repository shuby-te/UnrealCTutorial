// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"
#include "EnemyAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "HpUserWidget.h"
#include "HPActorComponent.h"

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

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());	 //GetRootComponent() -> GetMesh()
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetDrawSize(FVector2D(200.f, 20.f));

	static ConstructorHelpers::FClassFinder<UHpUserWidget> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
	}


	HpActorComponent = CreateDefaultSubobject<UHPActorComponent>(TEXT("HP Actor Component"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
	

	auto HpWidget = Cast<UHpUserWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(HpActorComponent);
	}

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
	
	HpActorComponent->OnDamaged(Damage);

	return Damage;
}

void AEnemy::EnemyAttack()
{
	if (IsValid(EnemyAnimInstance))
	{
		if (!IsAttacking)
		{
			EnemyAnimInstance->PlayAttackMontage();
			IsAttacking = true;

		}
	}
}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterupted)
{
	IsAttacking = false;
}

