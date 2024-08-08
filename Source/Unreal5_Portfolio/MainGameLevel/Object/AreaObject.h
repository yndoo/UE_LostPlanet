// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGameLevel/Object/MapObjectBase.h"
#include "AreaObject.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5_PORTFOLIO_API AAreaObject : public AMapObjectBase
{
	GENERATED_BODY()

public:
	void InterAction() override;

	FORCEINLINE void ResetBombTime()
	{
		InstallBombTime = 3.0f;
	}

	FORCEINLINE float GetInstallBombTime() const
	{
		return InstallBombTime;
	}

	void InstallBomb(float _DeltaTime);

	UFUNCTION(Reliable, Server)
	void BombPlanting(FName _InfoName);
	void BombPlanting_Implementation(FName _InfoName);

protected:
	AAreaObject();
	
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	


private:
	/// <summary>
	/// ��ź ��ġ �ð�
	/// </summary>
	float InstallBombTime = 3.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BombMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PlantingSpotCollision = nullptr;

};
