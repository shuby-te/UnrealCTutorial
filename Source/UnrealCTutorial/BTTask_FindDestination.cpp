// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindDestination.h"

UBTTask_FindDestination::UBTTask_FindDestination()
{
	NodeName = TEXT("Find destination");
}

EBTNodeResult::Type UBTTask_FindDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}
