// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"  // Ãß°¡
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCTUTORIAL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainWidgetClass;
	UPROPERTY()
	UUserWidget* MainWidget;
public:
	virtual void BeginPlay() override;
	
};
