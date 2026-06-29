// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCTUTORIAL_API UHpUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class UHPActorComponent* HpActorComp;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HP_ProgressBar;
public:
	void BindHp(class UHPActorComponent* ActorComp);
	void UpdateHP();
};
