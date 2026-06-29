// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPActorComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(함수포인터이름)
DECLARE_MULTICAST_DELEGATE(FOnHpChanged)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCTUTORIAL_API UHPActorComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float MaxHp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float Hp;
public:
	//함수포인터
	FOnHpChanged OnHpChanged; // -	void UpdateHP();
public:	
	UHPActorComponent();

protected:
	virtual void BeginPlay() override;
public:
	void OnDamaged(float DamagedAmount);
	float GetHp() const { return Hp; }
	void SetHp(float NewHp);
	float GetHpRatio() const;

};
