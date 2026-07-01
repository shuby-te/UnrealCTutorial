// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"


AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ACharacter> BP_Character(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Player.BP_Player_C'"));

	if (BP_Character.Succeeded())
	{
		DefaultPawnClass = BP_Character.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> BP_PlayerController(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_PlayerController.BP_PlayerController_C'"));
	if (BP_PlayerController.Succeeded())
	{
		PlayerControllerClass = BP_PlayerController.Class;
	}
}
