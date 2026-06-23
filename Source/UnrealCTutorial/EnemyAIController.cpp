// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AEnemyAIController::AEnemyAIController()
{
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
			UE_LOG(LogTemp, Log, TEXT("Run Behavior Tree"));
		}
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
