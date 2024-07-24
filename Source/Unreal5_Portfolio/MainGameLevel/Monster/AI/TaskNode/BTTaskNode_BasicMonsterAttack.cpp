// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Monster/AI/TaskNode/BTTaskNode_BasicMonsterAttack.h"
#include "MainGameLevel/Monster/Base/BasicMonsterBase.h"
#include "MainGameLevel/Player/MainPlayerState.h"
#include "MainGameLevel/Player/MainCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Global/ContentsLog.h"

EBTNodeResult::Type UBTTaskNode_BasicMonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABasicMonsterBase* Monster = GetSelfActor<ABasicMonsterBase>(OwnerComp);
	if (false == Monster->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, "Monster Is Not Valid");
		return EBTNodeResult::Type::Aborted;
	}

	UMonsterData* MonsterData = GetValueAsObject<UMonsterData>(OwnerComp, "MonsterData");
	if (false == MonsterData->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, "MonsterData Is Not Valid");
		return EBTNodeResult::Type::Aborted;
	}

	Monster->ChangeRandomAnimation(EBasicMonsterAnim::Attack);

	return EBTNodeResult::Type::InProgress;
}

void UBTTaskNode_BasicMonsterAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* pNodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, pNodeMemory, DeltaSeconds);

	// 플레이어 체크
	AMainCharacter* TargetPlayer = GetValueAsObject<AMainCharacter>(OwnerComp, "TargetActor");
	if (nullptr == TargetPlayer)
	{
		StateChange(OwnerComp, EBasicMonsterState::Idle);
		return;
	}

	// 사망한 플레이어일시 Idle
	AMainPlayerState* TargetPlayerState = Cast<AMainPlayerState>(TargetPlayer->GetPlayerState());
	if (0.0f >= TargetPlayerState->GetPlayerHp())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetActor", nullptr);
		StateChange(OwnerComp, EBasicMonsterState::Idle);
		return;
	}

	ABasicMonsterBase* Monster = GetSelfActor<ABasicMonsterBase>(OwnerComp);
	UMonsterData* MonsterData = GetValueAsObject<UMonsterData>(OwnerComp, "MonsterData");

	FVector MonsterLocation = Monster->GetActorLocation();
	FVector TargetPlayerLocation = TargetPlayer->GetActorLocation();

	FVector LocationDiff = TargetPlayerLocation - MonsterLocation;
	float Distance = LocationDiff.Size();
	if (Distance >= MonsterData->AttackBoundary)
	{
		// 몬스터 회전
		FRotator TurnRot = UKismetMathLibrary::FindLookAtRotation(MonsterLocation, TargetPlayerLocation);
		Monster->SetActorRotation(TurnRot);
	}
	else
	{
		StateChange(OwnerComp, EBasicMonsterState::Chase);
		return;
	}
}
