// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Monster/AI/TaskNode/BTTaskNode_BasicMonsterPatrol.h"
#include "MainGameLevel/Monster/AI/BasicMonsterAIController.h"
#include "MainGameLevel/Monster/Base/BasicMonsterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

#include "Global/ContentsLog.h"

EBTNodeResult::Type UBTTaskNode_BasicMonsterPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABasicMonsterBase* Monster = GetSelfActor<ABasicMonsterBase>(OwnerComp);
	if (false == Monster->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, "Monster Is Not Valid");
		return EBTNodeResult::Type::Aborted;
	}

	// Find PatrolLocation
	UMonsterData* MonsterData = GetValueAsObject<UMonsterData>(OwnerComp, "MonsterData");
	if (false == MonsterData->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, "MonsterData Is Not Valid");
		return EBTNodeResult::Type::Aborted;
	}

	FNavLocation PatrolLocation(FVector::ZeroVector);
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	bool IsFind = NavSystem->GetRandomReachablePointInRadius(MonsterData->OriginPos, MonsterData->Max_PatrolRange, PatrolLocation);
	if (false == IsFind)
	{
		return EBTNodeResult::Type::Failed;
	}

	SetValueAsVector(OwnerComp, "Destination", PatrolLocation.Location);
	Monster->GetCharacterMovement()->MaxWalkSpeed = Monster->GetBaseData()->GetWalkSpeed();
	Monster->ChangeRandomAnimation(EBasicMonsterAnim::Walk);

	return EBTNodeResult::Type::InProgress;
}

void UBTTaskNode_BasicMonsterPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* pNodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, pNodeMemory, DeltaSeconds);

	// TargetActor 존재시 Chase 상태로
	AActor* TargetActor = GetValueAsObject<AActor>(OwnerComp, "TargetActor");
	if (nullptr != TargetActor)
	{
		StateChange(OwnerComp, EBasicMonsterState::Chase);
		return;
	}

	// 목적지 도달 시 Idle
	ABasicMonsterBase* Monster = GetSelfActor<ABasicMonsterBase>(OwnerComp);
	if (false == Monster->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, "Monster Is Not Valid");
		return;
	}

	FVector PatrolLocation = GetValueAsVector(OwnerComp, "Destination");
	EPathFollowingRequestResult::Type IsMove = Monster->GetAIController()->MoveToLocation(PatrolLocation);
	if (EPathFollowingRequestResult::Type::AlreadyAtGoal == IsMove)
	{
		StateChange(OwnerComp, EBasicMonsterState::Idle);
		return;
	}
}
