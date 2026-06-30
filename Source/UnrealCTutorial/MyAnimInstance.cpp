// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyPlayer.h"  //교체
#include "GameFramework/CharacterMovementComponent.h"


UMyAnimInstance::UMyAnimInstance()
{
	//경로 수정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage"));

	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	auto Pawn = TryGetPawnOwner();					

	if (IsValid(Pawn))
	{
		//AMyPlayer 교체
		Character = Cast<AMyPlayer>(Pawn);

		if (IsValid(Character))
		{
			CharacterMovement = Character->GetCharacterMovement();
		}
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (IsValid(CharacterMovement))
	{
		Velocity = CharacterMovement->Velocity;
		GroundSpeed = Velocity.Size2D();
		FRotator Rotation = Character->GetActorRotation();
		FVector UnrotateVector = Rotation.UnrotateVector(Velocity);
		UnrotateVector.Normalize();

		Vertical = UnrotateVector.X;
		Horizontal = UnrotateVector.Y;


		auto Acceleration = CharacterMovement->GetCurrentAcceleration();

		ShouldMove = GroundSpeed > 0.1 && Acceleration != FVector::Zero();

		IsFalling = CharacterMovement->IsFalling();
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	if (IsValid(AttackMontage))
	{
		if (!Montage_IsPlaying(AttackMontage))
		{
			Montage_Play(AttackMontage);

			Character->PlayerAttack();
		}
	}
}
