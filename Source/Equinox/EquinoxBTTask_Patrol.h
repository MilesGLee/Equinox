// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EquinoxBTTask_Patrol.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
/// <summary>
/// MEans to assign a patrol path to the enemies 
/// </summary>
class EQUINOX_API UEquinoxBTTask_Patrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UEquinoxBTTask_Patrol();
private:

	/// <summary>
	/// Executes the task successful or not 
	/// </summary>
	/// <param name="Owner"></param>
	/// <param name="NodeMemory"></param>
	/// <returns></returns>
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory) override;

	/// <summary>
	/// Gets then displays the key name on the editor 
	/// </summary>
	/// <returns>FString</returns>
	virtual FString GetStaticDescription() const override;


	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/// <summary>
	/// If no pathpoaint is assigned to the enemy it'll assign a random point 
	/// </summary>
	/// <param name="Owner">compoennets owner</param>
	/// <param name="NodeMemory"></param>
	/// <returns></returns>
	EBTNodeResult::Type RandomPatrolPoints(UBehaviorTreeComponent& Owner, uint8* NodeMemory);

	/// <summary>
	/// If no pathpoaint is assigned to the enemy it'll assign a a point on there path point array
	/// </summary>
	/// <param name="Owner">compoennets owner</param>
	/// <param name="NodeMemory"></param>
	/// <returns></returns>
	EBTNodeResult::Type PlannedPatrolPoints(UBehaviorTreeComponent& Owner, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
		float m_serachRadius{ 500.0f };

	//iterates though the enemies PAth Point array 
	int m_pathIndex = 0;
};