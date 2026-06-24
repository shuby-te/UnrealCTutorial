// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindDestination.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindDestination::UBTTask_FindDestination()
{
	NodeName = TEXT("Find destination");
}

EBTNodeResult::Type UBTTask_FindDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	//if (IsValid(Pawn))
	if (Pawn != nullptr)
	{
		auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//만약에 UNavigationSystemV1 있다면
		if (NavSystem != nullptr)
		{
			FNavLocation RandomLocation;

			//만약에 도달할 위치 값을 찾았다면 
			//GetRandomReachablePointInRadius(중심점, 반지름, OUT 담을 위치)
			if(NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), 1000.f, OUT RandomLocation))
			{
				//GetBlackboardComponent()->SetValueAsVector("블랙보드 키 이름", 넣어줄 값)
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("Destination"), RandomLocation);
		
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
