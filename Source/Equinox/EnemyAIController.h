// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "EnemyAIController.generated.h"

UENUM()
enum class EnemyStats
{
	IDLE UMETA(DisplayName = "Idle"),
	ALERTED UMETA(DisplayName = "Alerted"),
	PURSUIT UMETA(DisplayName = "Pursue"),
	COLLAPSED UMETA(DisplayName = "Collapsed")
};

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class EQUINOX_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public: 
	AEnemyAIController();

protected:
	/// <summary>
	/// Updates once before Tick
	/// </summary>
	virtual void BeginPlay() override;

	/// <summary>
	/// Once pawn is assigned to this AIController
	/// </summary>
	/// <param name="pawn"></param>
	virtual void OnPossess(APawn* pawn) override;

	/// <summary>
	/// Updates once per frame
	/// </summary>
	/// <param name="DeltaSeconds"></param>
	virtual void Tick(float DeltaSeconds) override;

	/// <summary>
	/// Get's the controllers rotation 
	/// </summary>
	/// <returns></returns>
	virtual FRotator GetControlRotation() const override;

public:
	/// <summary>
	/// Gathers player positsion 
	/// </summary>
	FVector PlayerPosistion;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIController", meta = (AllowPrivateAccess = true))
	/// <summary>
	/// Conduct task and conditions for the owner of this AIController
	/// </summary>
	UBehaviorTree* m_behaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIController", meta = (AllowPrivateAccess = true))
	/// <summary>
	/// Attaches to the behavior tree 
	/// </summary>
	TSoftObjectPtr<UBehaviorTreeComponent> m_behaviorTreeComponenet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIController", meta = (AllowPrivateAccess = true))
	/// <summary>
	/// Container for the Blackboard Keys 
	/// </summary>
	TSoftObjectPtr<UBlackboardComponent> m_blackboardComponent;
};
