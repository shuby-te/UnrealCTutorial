// Fill out your copyright notice in the Description page of Project Settings.


#include "HpUserWidget.h"
#include "Components/ProgressBar.h"
#include "HPActorComponent.h"


void UHpUserWidget::BindHp(UHPActorComponent* ActorComp)
{
	//HPActorComponent 등록해서
	HpActorComp = ActorComp;
	//HPActorComponent 에 있는 OnHpChanged 함수포인터에 UpdqteHP 함수 등록
	HpActorComp->OnHpChanged.AddUObject(this, &UHpUserWidget::UpdateHP);

	//한번 업데이트 실행
	UpdateHP();
}

void UHpUserWidget::UpdateHP()
{
	HP_ProgressBar->SetPercent(HpActorComp->GetHpRatio());
}
