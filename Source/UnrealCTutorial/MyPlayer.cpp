#include "MyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"	
#include "EnhancedInputSubsystems.h"
#include "MyAnimInstance.h"
#include "Arrow.h"
#include "Kismet/GameplayStatics.h" 
#include "Components/WidgetComponent.h"
#include "HpUserWidget.h"
#include "HPActorComponent.h"

AMyPlayer::AMyPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -90.0), FRotator(0.0, -90.0, 0.0));

	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0, 0.0, 100.0), FRotator(-25.0, 0.0, 0.0));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.0, 120.0, 0.0);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Player.ABP_Player_C'"));

	if (AI.Succeeded())
	{
		GetMesh()->SetAnimClass(AI.Class);
	}

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(RootComponent);
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetDrawSize(FVector2D(400.f, 20.f));

	static ConstructorHelpers::FClassFinder<UHpUserWidget> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
	}


	HpActorComponent = CreateDefaultSubobject<UHPActorComponent>(TEXT("HP Actor Component"));

}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	auto HpWidget = Cast<UHpUserWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(HpActorComponent);
	}
	
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	} 

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayer::Look);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMyPlayer::Fire);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayer::Jump);

	}
	

}

float AMyPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HpActorComponent->OnDamaged(Damage);

	return Damage;
}

void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0, Rotation.Yaw, 0.0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	
}

void AMyPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}

void AMyPlayer::Fire(const FInputActionValue& Value)
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->PlayAttackMontage();

		float AttackRange = 100000.f;

		FHitResult HitResut;

		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);

		FVector AimLocation = CameraManager->GetCameraLocation();
		FVector TargetLocation = AimLocation + CameraManager->GetActorForwardVector() * AttackRange;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool Result = GetWorld()->LineTraceSingleByChannel
		(
			OUT HitResut,
			AimLocation,
			TargetLocation,
			ECollisionChannel::ECC_Visibility,
			Params
		);

		if (Result)
		{
			TargetLocation = HitResut.ImpactPoint;
		}

		FColor Color = Result ? FColor::Green : FColor::Red;

		DrawDebugLine(GetWorld(), AimLocation, TargetLocation, Color, true);

		FTransform SocketTransform = GetMesh()->GetSocketTransform(FName("ArrowSocket"));
		SocketLocation = SocketTransform.GetLocation();
		FVector DeltaVector = TargetLocation - SocketLocation;
		SocketRotation = FRotationMatrix::MakeFromX(DeltaVector).Rotator();
	}

	
}


void AMyPlayer::PlayerAttack()
{

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;

	auto MyArrow = GetWorld()->SpawnActor<AArrow>(SocketLocation, SocketRotation, Params);
}
