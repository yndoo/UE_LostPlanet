// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameLevel/Monster/BasicMonster/ClimbZombie/AI/BTTaskNode_ClimbZombieClimbEnd.h"
#include "MainGameLevel/Monster/BasicMonster/ClimbZombie/ClimbZombie.h"
#include "MainGameLevel/Monster/BasicMonster/ClimbZombie/ClimbZombeData.h"
#include "MainGameLevel/Monster/Animation/MonsterRandomAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Global/ContentsLog.h"

EBTNodeResult::Type UBTTaskNode_ClimbZombieClimbEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AClimbZombie* ClimbZombie = GetSelfActor<AClimbZombie>(OwnerComp);
	if (false == ClimbZombie->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, TEXT("ClimbZombie Is Not Valid"));
		return EBTNodeResult::Aborted;
	}

	UClimbZombeData* ClimbZombieData = ClimbZombie->GetSettingData();
	if (false == ClimbZombieData->IsValidLowLevel())
	{
		LOG(MonsterLog, Fatal, TEXT("ClimbZombieData Is Not Valid"));
		return EBTNodeResult::Aborted;
	}

	ClimbZombie->ChangeRandomAnimation(EBasicMonsterAnim::ClimbEnd);
	UMonsterRandomAnimInstance* AnimInst = ClimbZombie->GetAnimInstance();
	ClimbZombieData->TimeCount = AnimInst->GetKeyAnimMontage(EBasicMonsterAnim::ClimbEnd, ClimbZombie->GetAnimIndex())->GetPlayLength();

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_ClimbZombieClimbEnd::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* pNodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, pNodeMemory, DeltaSeconds);

	AClimbZombie* ClimbZombie = GetSelfActor<AClimbZombie>(OwnerComp);
	UClimbZombeData* ClimbZombieData = ClimbZombie->GetSettingData();

	if (0.0f >= ClimbZombieData->TimeCount)
	{
		ClimbZombie->GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
		StateChange(OwnerComp, EBasicMonsterState::Chase);
		return;
	}

	ClimbZombieData->TimeCount -= DeltaSeconds;
}
