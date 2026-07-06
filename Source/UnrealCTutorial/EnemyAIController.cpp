// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h" // 블랙보드 컴포넌트 접근용

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;	//Tick 호라성화

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/AI/BT_Enemy.BT_Enemy"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("/Game/AI/BB_Enemy.BB_Enemy"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(BehaviorTree))
	{
		if (RunBehaviorTree(BehaviorTree))
		{
			//UE_LOG(LogTemp, Log, TEXT("Run Behavior Tree"));
		}
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetBlackboardComponent())
	{
		auto TargetActor = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
		if (TargetActor)
		{
			//타겟이 있을 경우 플레이어 처다보기
			SetFocus(TargetActor);
		}
		else
		{
			//타겟이 없다면 조준 해제
			ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}
