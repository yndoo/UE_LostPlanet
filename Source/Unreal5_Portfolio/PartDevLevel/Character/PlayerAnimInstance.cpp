// Fill out your copyright notice in the Description page of Project Settings.


#include "PartDevLevel/Character/PlayerAnimInstance.h"
#include "TestLevel/Character/TestCharacter.h"
#include "MainGameLevel/Player/MainCharacter.h"
#include "Global/MainGameBlueprintFunctionLibrary.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerPlayer = Cast<ATestCharacter>(GetOwningActor());
	//OwnerPlayer = Cast<AMainCharacter>(GetOwningActor()); // Main - kjb

	// MainGameInst의 PlayerData에 저장된 Montages를 PlayerUpperMontages로 Add
	MainGameInst = UMainGameBlueprintFunctionLibrary::GetMainGameInstance(GetWorld());
	TMap<EPlayerPosture, class UAnimMontage*> PlayerUpperMontages = MainGameInst->GetPlayerData(FName("TestPlayer"))->GetAnimMontages();
	for (TPair<EPlayerPosture, class UAnimMontage*> Montage : PlayerUpperMontages)
	{
		PushAnimation(Montage.Key, Montage.Value);
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (nullptr == OwnerPlayer)
	{
		return;
	}

	PlayerPosture = OwnerPlayer->PostureValue;
	PlayerLowerState = OwnerPlayer->LowerStateValue;
	PlayerDir = OwnerPlayer->DirValue;
}

void UPlayerAnimInstance::ChangeAnimation(EPlayerPosture _Posture)
{
	Super::ChangeAnimation(_Posture);
}
