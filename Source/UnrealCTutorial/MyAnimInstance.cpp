// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


UMyAnimInstance::UMyAnimInstance()
{
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

		AimRotation = Character->GetBaseAimRotation();

		FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(Velocity);

		FRotator DeltaRotation = RotFromX - AimRotation;
		DeltaRotation.Normalize();

		YawOffset = DeltaRotation.Yaw;

		if (ShouldMove || IsFalling)
		{
			//움직였을때 RotateYaw값을 0으로 자연스럽게
			RotateYaw = FMath::FInterpTo(RotateYaw, 0.f, DeltaSeconds, 20.f);
			MovingRotation = Character->GetActorRotation();
			PrevRotation = MovingRotation;

		}
		else
		{
			PrevRotation = MovingRotation;
			MovingRotation = Character->GetActorRotation();
			FRotator DeltaMovingRotation = MovingRotation - PrevRotation;
			DeltaMovingRotation.Normalize();
			RotateYaw -= DeltaMovingRotation.Yaw;

			float TurnValue = GetCurveValue("Turn");
			if (TurnValue > 0.f)
			{
				PrevDistanceCurve = DistanceCurve;
				DistanceCurve = GetCurveValue("DistanceCurve");
				DeltaDistanceCurve = DistanceCurve - PrevDistanceCurve;

				if (RotateYaw > 0.f)
				{
					RotateYaw -= DeltaDistanceCurve;
				}
				else
				{
					RotateYaw += DeltaDistanceCurve;
				}

				float AbsRotateYawOffset = FMath::Abs(RotateYaw);
				if (AbsRotateYawOffset > 0.f)
				{
					float YawEcess = AbsRotateYawOffset - 90.f;
					if (RotateYaw > 0.f)
					{
						RotateYaw -= YawEcess;
					}
					else
					{
						RotateYaw += YawEcess;
					}
				}
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Rotation : %f"), RotateYaw);

	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	if (IsValid(AttackMontage))
	{
		if (!Montage_IsPlaying(AttackMontage))
		{
			Montage_Play(AttackMontage);
		
		}
	}
}

void UMyAnimInstance::AnimNotify_Shoot()
{
	Character->PlayerAttack();
}
