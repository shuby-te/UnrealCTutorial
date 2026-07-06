// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"
#include "EnemyAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "HpUserWidget.h"
#include "HPActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h" // CharacterMovement 사용할려면

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
	HpBar->SetupAttachment(RootComponent);
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetDrawSize(FVector2D(400.f, 20.f));

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

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

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

void AEnemy::EnemyHit()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 200.f;
	float AttackRadius = 40.f;
	float AttackHalfHeight = 90.f;

	FVector StartPos = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector FwdVector = Forward * AttackRange;

	FVector EndPos = StartPos + FwdVector;

	//캡슐을 캐릭터가 바라보는 방향으로 90도 회전.
	FQuat SweepRotation = FRotationMatrix::MakeFromZ(Forward).ToQuat();



	bool Result = GetWorld()->SweepSingleByChannel
	(
		OUT HitResult,
		StartPos,
		EndPos,
		SweepRotation,   //FQuat::Identity -> SweepRotation 실제 누운값으로 회전
		ECC_GameTraceChannel1,
		FCollisionShape::MakeCapsule(AttackRadius, AttackHalfHeight),
		Params
	);

	//디버그용 회전은 실제 회전값
	FQuat DebugRotation = SweepRotation;
	FColor DebugColor = Result ? FColor::Green : FColor::Red;

	FVector Center = StartPos + FwdVector * 0.5f;

	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		AttackHalfHeight,
		AttackRadius,
		DebugRotation,
		DebugColor,
		false,
		2.0f

	);

	if (Result && HitResult.GetActor())
	{
		//AActor* Target = HitResult.GetActor();
		auto Target = HitResult.GetActor();

		UGameplayStatics::ApplyDamage(Target, 10.f, GetController(), this, NULL);
	}



}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterupted)
{
	IsAttacking = false;
}

