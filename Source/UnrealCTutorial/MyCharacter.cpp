#include "MyCharacter.h"

AMyCharacter::AMyCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone"));

	//SkeletaMesh를 성공적으로 찾았다면
	if (SM.Succeeded())
	{
		//USkeletalMeshComponent에 있는 SkeletalMesh에 해당 오브젝트 넣어주기.
		GetMesh()->SetSkeletalMesh(SM.Object);

	}


}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

